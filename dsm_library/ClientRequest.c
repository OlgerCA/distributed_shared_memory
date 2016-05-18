#include <stddef.h>
#include "ClientRequest.h"

// This file should contain all socket related logic to communicate with server.
// Not sure if responses should be waited async.

NodeInitResponse *client_request_node_init(NodeInitRequest *request) {
    return NULL;
}

NodeExitResponse *client_request_node_exit(NodeExitRequest *request) {
    return NULL;
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
