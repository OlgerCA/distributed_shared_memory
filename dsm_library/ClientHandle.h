
#ifndef DISTRIBUTED_SHARED_MEMORY_CLIENTHANDLE_H
#define DISTRIBUTED_SHARED_MEMORY_CLIENTHANDLE_H

#include <Responses.h>
#include <Requests.h>

PageResponse* client_handle_page_request(PageRequest *request);
InvalidationResponse* client_handle_invalidation(InvalidationRequest *request);

#endif
