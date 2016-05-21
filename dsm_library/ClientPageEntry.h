
#ifndef DISTRIBUTED_SHARED_MEMORY_CLIENTPAGEENTRY_H
#define DISTRIBUTED_SHARED_MEMORY_CLIENTPAGEENTRY_H

#include <stdbool.h>

typedef struct {
    bool present;
    bool ownership;
    bool isReadOnly;
} ClientPageEntry;

#endif
