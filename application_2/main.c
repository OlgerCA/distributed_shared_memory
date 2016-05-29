
#include <stdio.h>
#include <DSMClient.h>

int main (int argc, char *argv[])
{
    puts("Hello from application_2");
    int numberOfNodes = 0;
    int nodeId = 0;
    DSM_node_init(&argc, &argv, &numberOfNodes, &nodeId);
    int* myArray = DSM_alloc(sizeof(int) * 1024);
    int result = myArray[1024];
    myArray[1] = 1;
    DSM_node_exit();
    return 0;
    return 0;
}