
#include "ServerPageEntry.h"

ServerPageEntry* server_page_entry_new(long pageNumber, ClientEntry* owner) {
    ServerPageEntry* this = (ServerPageEntry*) g_malloc(sizeof(ServerPageEntry));
    this->pageNumber = pageNumber;
    this->owner = owner;
    this->clientsWithCopies = NULL;
    return this;
}

void server_page_entry_free(ServerPageEntry* this) {
    g_slist_free(this->clientsWithCopies);
    g_free(this);
}
