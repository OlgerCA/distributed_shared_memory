#include <stddef.h>
#include "ServerHandle.h"

NodeInitResponse *server_handle_node_init(NodeInitRequest *request) {
    return NULL;
}

NodeExitResponse *server_handle_node_exit(NodeExitRequest *request) {
    return NULL;
}

AllocResponse *server_handle_alloc(AllocRequest *request) {
    return NULL;
}

void server_handle_page_request(PageRequest *request) {
}

void server_handle_invalidation(InvalidationRequest *request) {
}

BarrierResponse *server_handle_barrier(BarrierRequest *request) {
    return NULL;
}
