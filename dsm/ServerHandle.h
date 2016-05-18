
#ifndef DISTRIBUTED_SHARED_MEMORY_SERVERHANDLE_H
#define DISTRIBUTED_SHARED_MEMORY_SERVERHANDLE_H

#include "Requests.h"
#include "Responses.h"

NodeInitResponse* server_handle_node_init(NodeInitRequest* request);
NodeExitResponse* server_handle_node_exit(NodeExitRequest* request);
AllocResponse* server_handle_alloc(AllocRequest* request);
PageResponse* server_handle_page_request(PageRequest *request);
PageResponse* server_handle_invalidation(InvalidationRequest* request);
BarrierResponse* server_handle_barrier(BarrierRequest* request);

#endif
