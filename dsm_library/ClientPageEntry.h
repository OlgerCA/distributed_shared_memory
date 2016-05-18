
#ifndef DISTRIBUTED_SHARED_MEMORY_CLIENTPAGEENTRY_H
#define DISTRIBUTED_SHARED_MEMORY_CLIENTPAGEENTRY_H

typedef struct {
    int present;
    int ownership;
    int isReadOnly;
} ClientPageEntry;

#endif
