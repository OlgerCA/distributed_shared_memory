
#ifndef DISTRIBUTED_SHARED_MEMORY_RESPONSES_H
#define DISTRIBUTED_SHARED_MEMORY_RESPONSES_H

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
} AllocResponse;

typedef struct
{
    int errorCode;
    char pageContents[];
} PageResponse;

typedef struct
{
    int errorCode;
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
