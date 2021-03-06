
#ifndef DISTRIBUTED_SHARED_MEMORY_RESPONSES_H
#define DISTRIBUTED_SHARED_MEMORY_RESPONSES_H

#include <stdbool.h>

typedef struct
{
    int errorCode;
    int assignedNodeId;
    int numberOfNodes;
    long numberOfPages;
} NodeInitResponse;

typedef struct
{
    int errorCode;
} NodeExitResponse;

typedef struct
{
    int errorCode;
    long address;
    bool servedFromCache;
} AllocResponse;

typedef struct
{
    int errorCode;
    char pageContents[4096]; //TODO, change for page size
} PageResponse;

typedef struct
{
    int errorCode;
    long pageNumber;
} InvalidationResponse;

typedef struct
{
    int errorCode;
} BarrierResponse;

typedef  struct
{
    int errorCode;
} ACKResponse;

#endif
