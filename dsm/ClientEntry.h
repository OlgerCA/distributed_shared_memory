
#ifndef DISTRIBUTED_SHARED_MEMORY_CLIENTENTRY_H
#define DISTRIBUTED_SHARED_MEMORY_CLIENTENTRY_H

#include <stdbool.h>

typedef struct {
    int clientId;
    char* clientName;
    char* clientPort;
    bool isActive;
} ClientEntry;

ClientEntry* client_entry_new(int clientId, char* clientName, char* clientPort);
void client_entry_free(ClientEntry* this);

#endif
