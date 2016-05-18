
#ifndef DISTRIBUTED_SHARED_MEMORY_SERVERFORWARD_H
#define DISTRIBUTED_SHARED_MEMORY_SERVERFORWARD_H

#include <Responses.h>
#include <Requests.h>

PageResponse* server_forward_page_request(PageRequest *request);
PageResponse* server_forward_invalidation(InvalidationRequest* request);

#endif
