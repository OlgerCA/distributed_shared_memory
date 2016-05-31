
#ifndef DISTRIBUTED_SHARED_MEMORY_CLIENTREQUEST_H
#define DISTRIBUTED_SHARED_MEMORY_CLIENTREQUEST_H

#include "Requests.h"
#include "Responses.h"
#include "NetworkInfo.h"

NodeInitResponse* client_request_node_init(NodeInitRequest* request, NetworkInfo* networkInfo);
NodeExitResponse* client_request_node_exit(NodeExitRequest* request);
AllocResponse* client_request_alloc(AllocRequest* request);
PageResponse client_request_page(PageRequest* request);
InvalidationResponse* client_request_invalidation(InvalidationRequest* request);
BarrierResponse* client_request_barrier(BarrierRequest request);

#endif
