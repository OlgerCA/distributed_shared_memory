#include <stddef.h>
#include <NetworkInfo.h>
#include <malloc.h>
#include "ClientRequest.h"

// This file should contain all socket related logic to communicate with server.
// Not sure if responses should be waited async.

static NetworkInfo* netInfo;

NodeInitResponse *client_request_node_init(NodeInitRequest *request, NetworkInfo* networkInfo) {
    netInfo = networkInfo;
    NodeInitResponse* response = (NodeInitResponse*) malloc(sizeof(NodeInitResponse));

    return response;
}

NodeExitResponse *client_request_node_exit(NodeExitRequest *request) {
    NodeExitResponse* response = (NodeExitResponse*) malloc(sizeof(NodeExitResponse));
    free(netInfo);

    return response;
}

AllocResponse *client_request_alloc(AllocRequest *request) {
    return NULL;
}

PageResponse *client_request_page(PageRequest *request) {
    return NULL;
}

InvalidationResponse *client_request_invalidation(InvalidationRequest *request) {
    return NULL;
}

BarrierResponse *client_request_barrier(BarrierRequest *request) {
    return NULL;
}
