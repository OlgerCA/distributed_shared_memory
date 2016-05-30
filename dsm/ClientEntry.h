
#ifndef DISTRIBUTED_SHARED_MEMORY_CLIENTENTRY_H
#define DISTRIBUTED_SHARED_MEMORY_CLIENTENTRY_H

#include <stdbool.h>

typedef struct {
    int clientId;
    int clientSocketId;
    bool isActive;
    char forwardIpAddress[20];
    int forwardPort;
} ClientEntry;

ClientEntry* client_entry_new(int clientId, int clientSocketId, char* forwardIpAddress, int forwardPort);
void client_entry_free(ClientEntry* this);

#endif
