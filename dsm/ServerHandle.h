
#ifndef DISTRIBUTED_SHARED_MEMORY_SERVERHANDLE_H
#define DISTRIBUTED_SHARED_MEMORY_SERVERHANDLE_H

#include "Requests.h"
#include "Responses.h"

void server_startup(long numberOfPages, int numberOfClients);
void server_teardown();

NodeInitResponse* server_handle_node_init(NodeInitRequest* request, char* clientName, char* clientPort);
NodeExitResponse* server_handle_node_exit(NodeExitRequest* request);
AllocResponse* server_handle_alloc(AllocRequest* request);
PageResponse* server_handle_page_request(PageRequest *request);
PageResponse* server_handle_invalidation(InvalidationRequest* request);
BarrierResponse* server_handle_barrier(BarrierRequest* request);

#endif
