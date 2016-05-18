
#ifndef DISTRIBUTED_SHARED_MEMORY_SERVERPAGEENTRY_H
#define DISTRIBUTED_SHARED_MEMORY_SERVERPAGEENTRY_H

typedef struct {
    long owner;
    long* copies;
};

#endif
