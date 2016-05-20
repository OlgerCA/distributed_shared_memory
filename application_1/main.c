
#include <stdio.h>
#include <DSMClient.h>

int main (int argc, char *argv[])
{
    puts("Hello from application_1");

    int numberOfNodes = 0;
    int nodeId = 0;
    DSM_node_init(&argc, &argv, &numberOfNodes, &nodeId);


    DSM_node_exit();
    return 0;
}