#include <stdlib.h>
#include <string.h>
#include "ClientEntry.h"

ClientEntry *client_entry_new(int clientId, char *clientName, char *clientPort) {
    ClientEntry* this = (ClientEntry*) malloc(sizeof(ClientEntry));
    this->clientId = clientId;
    this->clientName = strdup(clientName);
    this->clientPort = strdup(clientPort);
    this->isActive = true;
    return this;
}

void client_entry_free(ClientEntry *this) {
    free(this->clientName);
    free(this->clientPort);
    free(this);
}
