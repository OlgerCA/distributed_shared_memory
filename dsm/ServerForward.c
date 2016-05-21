#include <stddef.h>
#include "ServerForward.h"
#include "ClientEntry.h"

PageResponse* server_forward_page_request(PageRequest *request, ClientEntry* owner) {
    return NULL;
}

InvalidationResponse* server_forward_invalidation(InvalidationRequest *request, ClientEntry* client) {
    return NULL;
}
