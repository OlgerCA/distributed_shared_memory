#include <stdio.h>
#include "ServerPageEntry.h"
#include "Logger.h"
#include "ClientEntry.h"

ServerPageEntry* server_page_entry_new(long pageNumber, ClientEntry* owner) {

    char logMessage[100];
    sprintf(logMessage, "New page entry page: %ld on %d", pageNumber, owner->clientId);

    logger_log_message(logMessage, INFO);
    ServerPageEntry* this = (ServerPageEntry*) g_malloc(sizeof(ServerPageEntry));
    this->pageNumber = pageNumber;
    this->owner = owner;
    sprintf(logMessage, "The original owner of page %ld is %d\n", this->pageNumber, this->owner->clientId);
    this->clientsWithCopies = NULL;
    return this;
}

void server_page_entry_free(ServerPageEntry* this) {
    g_slist_free(this->clientsWithCopies);
    g_free(this);
}
