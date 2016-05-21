
#ifndef DISTRIBUTED_SHARED_MEMORY_SERVERFORWARD_H
#define DISTRIBUTED_SHARED_MEMORY_SERVERFORWARD_H

#include <Responses.h>
#include <Requests.h>
#include "ClientEntry.h"

PageResponse* server_forward_page_request(PageRequest *request, ClientEntry* owner);
InvalidationResponse* server_forward_invalidation(InvalidationRequest* request, ClientEntry* client);

#endif
