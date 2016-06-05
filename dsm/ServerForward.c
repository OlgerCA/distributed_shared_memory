#include <stddef.h>
#include <stdlib.h>
#include <NetworkInfo.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <Responses.h>
#include <zconf.h>
#include "ServerForward.h"
#include "ClientEntry.h"
#include "Server.h"
#include "Logger.h"

PageResponse* server_forward_page_request(int client, PageRequest *request, ClientEntry* owner) {
    int clientWithPageSocket = server_connect(owner->forwardIpAddress, owner->forwardPort);

    char logMessage[100];
    sprintf(logMessage, "Forwarding request to %s:%d", owner->forwardIpAddress, owner->forwardPort);

    logger_log_message(logMessage, INFO);
    
    char message[MAXDATASIZE];
    int buffer1 = 0;
    int buffer2 = 0;
    long buffer3 = 0;

    sprintf(
            message,
            REQ_FORMAT,
            GET,
            PAGE,
            request->nodeId,
            request->ownershipOnly,
            request->readOnlyMode,
            request->pageNumber
    );

    printf("dperez, message sent: %s \n", message);

    send(clientWithPageSocket, message, strlen(message), 0);
    recv(clientWithPageSocket, message, MAXDATASIZE, 0);

    printf("dperez, message recv: %s \n", message);
    PageResponse* response = (PageResponse*) malloc(sizeof(PageResponse));

    sscanf(
            message,
            RES_FORMAT,
            &response->errorCode,
            &buffer1,
            &buffer2,
            &buffer3,
            response->pageContents
    );

    // requesting not only ownership and no error
    if (!request->ownershipOnly && response->errorCode == 0) {
        char *contentBeforePage = strchr(message, '&');
        memcpy(response->pageContents, contentBeforePage + 1, getpagesize());
    }

    shutdown(clientWithPageSocket, SHUT_RDWR);
    close(clientWithPageSocket);
    return response;
}

InvalidationResponse* server_forward_invalidation(InvalidationRequest *request, ClientEntry* client) {
    int cx = server_connect(client->forwardIpAddress, client->forwardPort);
    if(cx == -1){
        printf("dperez, Retorna -1\n");
    }

    char logMessage[100];
    sprintf(logMessage, "Invalidating page to %s:%d", client->forwardIpAddress, client->forwardPort);

    logger_log_message(logMessage, INFO);
    
    char message[MAXDATASIZE];
    int buffer1 = 0;
    int buffer2 = 0;
    long buffer3 = 0;
    char buffer4[2];

    sprintf(message, REQ_FORMAT, GET, INVA, request->nodeId, ZERO, ZERO, request->pageNumber);

    send(cx, message, strlen(message), 0);
    recv(cx, message, MAXDATASIZE, 0);

    InvalidationResponse* response = (InvalidationResponse*) malloc(sizeof(InvalidationResponse));

    sscanf(
            message,
            RES_FORMAT,
            &response->errorCode,
            &buffer1,
            &buffer2,
            &buffer3,
            buffer4
    );

    shutdown(cx, SHUT_RDWR);
    close(cx);
    return response;
}
