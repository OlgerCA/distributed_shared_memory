
#include "DSMClient.h"

// This file should contain all memory protection and mapping logic.
// This is the only API used by client applications.

int DSM_node_init(int *argc, char ***argv, int *nodes, int *nid) {
    return 0;
}

int DSM_node_exit(void) {
    return 0;
}

void *DSM_alloc(size_t size) {
    return NULL;
}

void DSM_barrier(int barrier_id) {

}
