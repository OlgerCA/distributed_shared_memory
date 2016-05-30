#include <stddef.h>
#include <stdlib.h>
#include <NetworkInfo.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <Responses.h>
#include "ServerForward.h"
#include "ClientEntry.h"
#include "Server.h"

PageResponse* server_forward_page_request(PageRequest *request, ClientEntry* owner) {
    int cx = server_connect(owner->forwardIpAddress, owner->forwardPort);
    
    char* message = (char*) malloc(MAXDATASIZE);
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

    send(cx, message, strlen(message), 0);
    recv(cx, message, MAXDATASIZE, 0);

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

    free(message);
    shutdown(cx, SHUT_RDWR);

    return response;
}

InvalidationResponse* server_forward_invalidation(InvalidationRequest *request, ClientEntry* client) {
    int cx = server_connect(client->forwardIpAddress, client->forwardPort);
    
    char* message = (char*) malloc(MAXDATASIZE);
    int buffer1 = 0;
    int buffer2 = 0;
    long buffer3 = 0;
    char* buffer4 = (char*) malloc(2);

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

    free(buffer4);
    free(message);
    shutdown(cx, SHUT_RDWR);

    return response;
}
