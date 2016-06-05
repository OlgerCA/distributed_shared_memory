
#ifndef DISTRIBUTED_SHARED_MEMORY_SERVERHANDLE_H
#define DISTRIBUTED_SHARED_MEMORY_SERVERHANDLE_H

#include "Requests.h"
#include "Responses.h"

void server_startup(long numberOfPages, int numberOfClients);
void server_teardown();

NodeInitResponse* server_handle_node_init(NodeInitRequest* request, int clientSocketId);
NodeExitResponse* server_handle_node_exit(NodeExitRequest* request);
AllocResponse* server_handle_alloc(AllocRequest* request);
PageResponse* server_handle_page_request(int client, PageRequest *request);
void server_handler_updateOwnerInfo(PageRequest *request);
InvalidationResponse* server_handle_invalidation(InvalidationRequest* request);
void server_handle_barrier(BarrierRequest* request);

#endif

