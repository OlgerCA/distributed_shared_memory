#include <stdio.h>
#include "ServerPageEntry.h"
#include "Logger.h"

ServerPageEntry* server_page_entry_new(long pageNumber, ClientEntry* owner) {

    char logMessage[100];
    sprintf(logMessage, "New page entry page: %ld on %d", pageNumber, owner->clientId);

    logger_log_message(logMessage, INFO);
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
