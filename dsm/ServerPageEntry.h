
#ifndef DISTRIBUTED_SHARED_MEMORY_SERVERPAGEENTRY_H
#define DISTRIBUTED_SHARED_MEMORY_SERVERPAGEENTRY_H

#include <glib.h>
#include "ClientEntry.h"

typedef struct {
    long pageNumber;
    ClientEntry* owner;
    GSList* clientsWithCopies;
} ServerPageEntry;

ServerPageEntry* server_page_entry_new(long pageNumber, ClientEntry* owner);
void server_page_entry_free(ServerPageEntry* this);

#endif
