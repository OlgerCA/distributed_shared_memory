#include <Requests.h>
#include <stdlib.h>
#include <Responses.h>
#include <NetworkInfo.h>
#include "DSMClient.h"
#include "ClientRequest.h"

// This file should contain all memory protection and mapping logic.
// This is the only API used by client applications.

static int nodeId;

// From arguments we should receive server ip address and port, and also the client port to use.
int DSM_node_init(int *argc, char ***argv, int *nodes, int *nid) {

    if (*argc <= 3)
        return -1;

    NetworkInfo* networkInfo = (NetworkInfo*) malloc(sizeof(NetworkInfo));
    networkInfo->serverName = (*argv)[1];
    networkInfo->serverPort = (*argv)[2];
    networkInfo->clientPort = (*argv)[3];

    *argc -= 4;
    *argv += 4;

    NodeInitRequest* request = (NodeInitRequest*) malloc(sizeof(NodeInitRequest));
    NodeInitResponse* response = client_request_node_init(request, networkInfo);

    if (response->errorCode != 0) {
        free(request);
        free(response);
        free(networkInfo);
        return -1;
    }

    *nodes = response->numberOfNodes;
    *nid = response->assignedNodeId;
    nodeId = response->assignedNodeId;

    // TODO Init page table and other data structures

    free(request);
    free(response);
    return 0;
}

int DSM_node_exit(void) {

    NodeExitRequest* request = (NodeExitRequest*) malloc(sizeof(NodeExitRequest));
    request->nodeId = nodeId;
    NodeExitResponse* response = client_request_node_exit(request);

    int errorCode = response->errorCode;
    free(request);
    free(response);
    nodeId = 0;

    // TODO Free Page table

    return errorCode != 0;
}

void *DSM_alloc(size_t size) {
    return NULL;
}

void DSM_barrier(int barrier_id) {

}
