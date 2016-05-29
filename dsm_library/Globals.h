
#ifndef DISTRIBUTED_SHARED_MEMORY_GLOBALS_H
#define DISTRIBUTED_SHARED_MEMORY_GLOBALS_H

#include <stddef.h>
#include "ClientPageEntry.h"

extern int nodeId;

extern ClientPageEntry* pages;
extern long totalNumberOfPages;

extern void* addressSpace;
extern size_t addressSpaceLength;

#endif
