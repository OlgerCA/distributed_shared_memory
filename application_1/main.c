
#include <stdio.h>
#include <DSMClient.h>
#define ARRAY_SIZE 4096

int main (int argc, char *argv[])
{
    int nodes, nid, i;
    if (DSM_node_init(&argc, &argv, &nodes, &nid) != 0) {
        puts("Error when initializing the node...");
        return -1;
    }

    int* numbers = (int*) DSM_alloc(sizeof(int) * ARRAY_SIZE);
    int* partialResults = (int*) DSM_alloc(sizeof(int) * nodes);
    if (nid == 0) {
        for (i = 0; i < ARRAY_SIZE; i++)
            numbers[i] = i;
    }

    DSM_barrier(0);
    int partialResult = 0;
    int assignedElements = ARRAY_SIZE / nodes;
    for (i = nid * assignedElements; i < (nid + 1) * assignedElements; i++)
        partialResult += numbers[i];
    partialResults[nid] = partialResult;

    DSM_barrier(1);
    if (nid == 0)  {
        int total = 0;
        for (i = 0; i < nodes; i++)
            total += partialResults[i];
        printf("The final result is: %d\n", total);
    }

    DSM_barrier(2);
    if (DSM_node_exit() != 0) {
        puts("Error when exiting from node...");
        return -1;
    }
    return 0;
}