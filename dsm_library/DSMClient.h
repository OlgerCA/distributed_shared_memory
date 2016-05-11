
#ifndef DISTRIBUTED_SHARED_MEMORY_DSMCLIENT_H
#define DISTRIBUTED_SHARED_MEMORY_DSMCLIENT_H

#include <stddef.h>

int DSM_node_init(int* argc, char** argv[], int* nodes, int* nid);
int DSM_node_exit(void);
void* DSM_alloc(size_t size);
void DSM_barrier(int barrier_id);

#endif
