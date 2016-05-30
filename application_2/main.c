
#include <stdio.h>
#include <DSMClient.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define ARRAY_SIZE 4096
#define MAX_VALUE 10000

int isPrime(int n){
    int i = 2, half = n/2;

    for(; i <= half; i++)
        if(n % i == 0)
            return 0;

    return 1;
}

int main (int argc, char *argv[])
{
    int nodes, nid, i;
    if (DSM_node_init(&argc, &argv, &nodes, &nid) != 0) {
        puts("Error when initializing the node...");
        return -1;
    }

    int* numbers = (int*) DSM_alloc(sizeof(int) * ARRAY_SIZE);
    int* partialResults = (int*) DSM_alloc(sizeof(int) * nodes);
    int secuentialTotal = 0;
    srand48(time(NULL));
    if (nid == 0) {
        for (i = 0; i < ARRAY_SIZE; i++) {
            numbers[i] = lrand48() % MAX_VALUE;;
            secuentialTotal += isPrime(numbers[i]);
        }
    }

    printf("The number of primer numbers secuentially found is %d\n", secuentialTotal);
    DSM_barrier(0);
    int partialResult = 0;
    int assignedElements = ARRAY_SIZE / nodes;
    for (i = nid * assignedElements; i < (nid + 1) * assignedElements; i++) {
        partialResult += isPrime(numbers[i]);
    }
    partialResults[nid] = partialResult;
    printf("The number of primer numbers found by: %d is: %d\n", nid, partialResult);

    DSM_barrier(1);
    if (nid == 0)  {
        int total = 0;
        for (i = 0; i < nodes; i++)
            total += partialResults[i];

        printf("The number of primer numbers found distributively in the array is: %d\n", total);
    }

    DSM_barrier(2);
    if (DSM_node_exit() != 0) {
        puts("Error when exiting from node...");
        return -1;
    }
    return 0;
}