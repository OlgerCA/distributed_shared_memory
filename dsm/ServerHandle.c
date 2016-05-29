#include <stddef.h>
#include <stdlib.h>
#include <Responses.h>
#include <Requests.h>
#include <unistd.h>
#include <math.h>
#include <NetworkInfo.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include "ServerHandle.h"
#include "ServerPageEntry.h"
#include "Logger.h"
#include "ServerForward.h"
#include "ClientEntry.h"

static ServerPageEntry** pages;
static ClientEntry** clients;
static long currentPage;
static int currentClient;

static long totalNumberOfPages;
static int totalNumberOfClients;

static int* mallocsPerClient;
static long* mallocAddresses;
static int lastMalloc;

static int barriers[1024];


void server_startup(long numberOfPages, int numberOfClients) {
    logger_log_message("Starting up server...", INFO);
    currentPage = -1;
    currentClient = -1;

    mallocsPerClient = (int*) malloc(sizeof(int) * numberOfClients);
    mallocAddresses = (long*) malloc(sizeof(long) * numberOfPages);
    lastMalloc = -1;
    int i;
    for (i = 0; i < numberOfClients; i++)
        mallocsPerClient[i] = -1;

    for (i = 0; i < 1024; i++){
        barriers[i] = numberOfClients;
    }

    pages = (ServerPageEntry**) malloc(sizeof(ServerPageEntry*) * numberOfPages);
    clients = (ClientEntry**) malloc(sizeof(ClientEntry*) * numberOfClients);
    totalNumberOfPages = numberOfPages;
    totalNumberOfClients = numberOfClients;
    logger_log_message("Starting up server finished.", INFO);
}

void server_teardown() {
    logger_log_message("Shutting down server...", INFO);
    int i;
    for (i = 0; i < currentPage; i++)
        server_page_entry_free(pages[i]);
    free(pages);
    for (i = 0; i < currentClient; i++)
        client_entry_free(clients[i]);
    free(clients);
    free(mallocsPerClient);
    free(mallocAddresses);
    currentPage = -1;
    currentClient = -1;
    logger_log_message("Shutting down server finished.", INFO);
}

NodeInitResponse *server_handle_node_init(NodeInitRequest *request, int clientSocketId) {
    NodeInitResponse* response = (NodeInitResponse*) malloc(sizeof(NodeInitResponse));
    if (currentClient == totalNumberOfClients - 1) {
        logger_log_message("More clients than originally allowed tried to register.", ERROR);
        response->errorCode = -1;
        return response;
    }

    ClientEntry* newClient = client_entry_new(++currentClient, clientSocketId);
    clients[currentClient] = newClient;

    response->errorCode = 0;
    response->assignedNodeId = currentClient;
    response->numberOfNodes = totalNumberOfClients;
    response->numberOfPages = totalNumberOfPages;
    logger_log_message("node_init request handled successfully.", INFO);
    return response;
}

NodeExitResponse *server_handle_node_exit(NodeExitRequest *request) {
    NodeExitResponse* response = (NodeExitResponse*) malloc(sizeof(NodeExitResponse));
    if (request->nodeId < 0 || request->nodeId >= totalNumberOfClients) {
        logger_log_message("Invalid client ID was supplied to server.", ERROR);
        response->errorCode = -2;
        return response;
    }
    clients[request->nodeId]->isActive = false;
    response->errorCode = 0;
    logger_log_message("node_exit request handled successfully.", INFO);
    return response;
}

AllocResponse *server_handle_alloc(AllocRequest *request) {
    AllocResponse* response = (AllocResponse*) malloc(sizeof(AllocResponse));

    if (request->nodeId < 0 || request->nodeId >= totalNumberOfClients) {
        logger_log_message("Invalid client ID was supplied to server.", ERROR);
        response->errorCode = -2;
        return response;
    }

    if (mallocsPerClient[request->nodeId] + 1 <= lastMalloc) {
        mallocsPerClient[request->nodeId]++;
        response->errorCode = 0;
        response->address = mallocAddresses[mallocsPerClient[request->nodeId]];
        response->servedFromCache = true;
        return response;
    }

    int pageSize = getpagesize();
    int numberOfRequestedPages = (int) ceil(request->size * 1.0 / pageSize);
    if (totalNumberOfPages - (currentPage + 1) < numberOfRequestedPages) {
        logger_log_message("Not enough memory available for perform allocation.", ERROR);
        response->errorCode = -3;
        return response;
    }

    int i;
    ClientEntry* requestingClient = clients[request->nodeId];
    long startingAddress = (currentPage + 1) * pageSize;
    for (i = 0; i < numberOfRequestedPages; i++) {
        currentPage++;
        pages[currentPage] = server_page_entry_new(currentPage, requestingClient);
    }
    response->errorCode = 0;
    response->address = startingAddress;
    response->servedFromCache = false;
    mallocsPerClient[request->nodeId]++;
    lastMalloc++;
    mallocAddresses[lastMalloc] = startingAddress;

    return response;
}


InvalidationResponse* invalidate_page(int nodeId, long pageNumber, ClientEntry* client) {
    InvalidationRequest *invalidationRequest = (InvalidationRequest *) malloc(sizeof(InvalidationRequest));
    invalidationRequest->nodeId = nodeId;
    invalidationRequest->pageNumber = pageNumber;
    InvalidationResponse *response = server_forward_invalidation(invalidationRequest, client);
    if (response->errorCode != 0)
        logger_log_message("Error invalidating page.", ERROR);

    free(invalidationRequest);
    return response;
}

void invalidate_page_wrapper(gpointer data, gpointer user_data) {
    ClientEntry* client = (ClientEntry*) data;
    PageRequest* request = (PageRequest*) user_data;

    if (client->clientId != request->nodeId) {
        InvalidationResponse* response = invalidate_page(request->nodeId, request->pageNumber, client);
        free(response);
    }
}

PageResponse* server_handle_page_request(PageRequest *request) {
    if (request->nodeId < 0 || request->nodeId >= totalNumberOfClients) {
        logger_log_message("Invalid client ID was supplied to server.", ERROR);
        PageResponse* response = (PageResponse*) malloc(sizeof(PageResponse));
        response->errorCode = -2;
        return response;
    }
    if (request->pageNumber < 0 || request->pageNumber >= currentPage + 1) {
        logger_log_message("Invalid page number was requested.", ERROR);
        PageResponse* response = (PageResponse*) malloc(sizeof(PageResponse));
        response->errorCode = -4;
        return response;
    }

    ServerPageEntry* pageEntry = pages[request->pageNumber];
    ClientEntry* owner = pageEntry->owner;

    PageResponse* response = server_forward_page_request(request, owner);
    if (response->errorCode != 0) {
        logger_log_message("Page forward request failed.", ERROR);
        return response;
    }

    if (request->readOnlyMode == false) {
        pageEntry->owner = clients[request->nodeId];
        g_slist_foreach(pageEntry->clientsWithCopies, invalidate_page_wrapper, request);
        g_slist_free(pageEntry->clientsWithCopies);
        pageEntry->clientsWithCopies = NULL;
    } else {
        pageEntry->clientsWithCopies = g_slist_prepend(pageEntry->clientsWithCopies, clients[request->nodeId]);
    }

    return response;
}

InvalidationResponse* server_handle_invalidation(InvalidationRequest *request) {
    if (request->nodeId < 0 || request->nodeId >= totalNumberOfClients) {
        logger_log_message("Invalid client ID was supplied to server.", ERROR);
        InvalidationResponse* response = (InvalidationResponse*) malloc(sizeof(InvalidationResponse));
        response->errorCode = -2;
        return response;
    }
    if (request->pageNumber < 0 || request->pageNumber >= currentPage + 1) {
        logger_log_message("Invalid page number was requested.", ERROR);
        InvalidationResponse* response = (InvalidationResponse*) malloc(sizeof(InvalidationResponse));
        response->errorCode = -4;
        return response;
    }

    ServerPageEntry* pageEntry = pages[request->pageNumber];
    g_slist_foreach(pageEntry->clientsWithCopies, invalidate_page_wrapper, request);
    g_slist_free(pageEntry->clientsWithCopies);
    pageEntry->clientsWithCopies = NULL;

    InvalidationResponse* response = (InvalidationResponse*) malloc(sizeof(InvalidationResponse));
    response->errorCode = 0; // TODO We might need to add an extra check.
    return response;
}

void server_handle_barrier(BarrierRequest *request) {
    int barrierId = request->barrierId;
    barriers[barrierId]--;
    logger_log_message("Barrier was requested", INFO);
    if(barriers[barrierId] == 0){
        int nodeIndex = 0;
        logger_log_message("Barrier was completed, answering to nodes", INFO);
        for(; nodeIndex < totalNumberOfClients; nodeIndex++){
            char* message = (char*) malloc(MAXDATASIZE);
            sprintf(
                    message,
                    RES_FORMAT,
                    ZERO,
                    ZERO,
                    ZERO,
                    (long) ZERO,
                    VOID
            );
            send(clients[nodeIndex]->clientSocketId, message, strlen(message), 0);
        }
    }
}
