#include <stddef.h>
#include <stdlib.h>
#include <Responses.h>
#include <Requests.h>
#include "ServerHandle.h"
#include "ServerPageEntry.h"
#include "ClientEntry.h"
#include "Logger.h"

static ServerPageEntry** pages;
static ClientEntry** clients;
static long currentPage;
static int currentClient;

static long totalNumberOfPages;
static int totalNumberOfClients;

void server_startup(long numberOfPages, int numberOfClients) {
    logger_log_message("Starting up server...", INFO);
    currentPage = -1;
    currentClient = -1;
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
    currentPage = -1;
    currentClient = -1;
    logger_log_message("Shutting down server finished.", INFO);
}

NodeInitResponse *server_handle_node_init(NodeInitRequest *request, char* clientName, char* clientPort) {
    NodeInitResponse* response = (NodeInitResponse*) malloc(sizeof(NodeInitResponse));
    if (currentClient == totalNumberOfClients - 1) {
        logger_log_message("More clients than originally allowed tried to register.", ERROR);
        response->errorCode = -1;
        return response;
    }

    ClientEntry* newClient = client_entry_new(++currentClient, clientName, clientPort);
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
    response->errorCode = 0;
    logger_log_message("node_exit request handled successfully.", INFO);
    return response;
}

AllocResponse *server_handle_alloc(AllocRequest *request) {
    return NULL;
}

PageResponse* server_handle_page_request(PageRequest *request) {
    return NULL;
}

PageResponse* server_handle_invalidation(InvalidationRequest *request) {
    return  NULL;
}

BarrierResponse *server_handle_barrier(BarrierRequest *request) {
    return NULL;
}
