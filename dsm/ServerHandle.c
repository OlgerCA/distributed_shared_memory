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

PageResponse* server_handle_page_request(PageRequest *request) {
    return NULL;
}

PageResponse* server_handle_invalidation(InvalidationRequest *request) {
    return  NULL;
}

BarrierResponse *server_handle_barrier(BarrierRequest *request) {
    return NULL;
}
