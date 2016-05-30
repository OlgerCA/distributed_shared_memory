#include <stdlib.h>
#include <string.h>
#include "ClientEntry.h"

ClientEntry *client_entry_new(int clientId, int clientSocketId, char* forwardIpAddress, int forwardPort) {
    ClientEntry* this = (ClientEntry*) malloc(sizeof(ClientEntry));
    this->clientId = clientId;
    this->clientSocketId = clientSocketId;
    strcpy(this->forwardIpAddress, forwardIpAddress);
    this->isActive = true;
    return this;
}

void client_entry_free(ClientEntry *this) {
    free(this);
}
