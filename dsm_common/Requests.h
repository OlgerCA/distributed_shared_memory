
#ifndef DISTRIBUTED_SHARED_MEMORY_REQUESTS_H
#define DISTRIBUTED_SHARED_MEMORY_REQUESTS_H

typedef struct
{
} NodeInitRequest;

typedef struct
{
    int nodeId;
} NodeExitRequest;

typedef struct
{
    int nodeId;
    long size;
} AllocRequest;

typedef struct
{
    int nodeId;
    long pageNumber;
    int ownershipOnly;
    int readOnlyMode;
} PageRequest;

typedef struct
{
    int nodeId;
    long pageNumber;
} InvalidationRequest;

typedef struct
{
    int nodeId;
    int barrierId;
} BarrierRequest;

#endif
