#include <stdlib.h>
#include <string.h>
#include "ClientEntry.h"
#include "Logger.h"

ClientEntry *client_entry_new(int clientId, int clientSocketId, char* forwardIpAddress, int forwardPort) {
    logger_log_message("Creating new client entry", INFO);
    ClientEntry* this = (ClientEntry*) malloc(sizeof(ClientEntry));
    this->clientId = clientId;
    this->clientSocketId = clientSocketId;
    strcpy(this->forwardIpAddress, forwardIpAddress);
    this->forwardPort = forwardPort;
    this->isActive = true;
    return this;
}

void client_entry_free(ClientEntry *this) {
    free(this);
}
