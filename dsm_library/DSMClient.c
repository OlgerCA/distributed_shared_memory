#include <sys/mman.h>
#include <Requests.h>
#include <stdlib.h>
#include <Responses.h>
#include <NetworkInfo.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include "DSMClient.h"
#include "ClientRequest.h"
#include "ClientPageEntry.h"

// This file should contain all memory protection and mapping logic.
// This is the only API used by client applications.

static int nodeId;

static ClientPageEntry* pages;
static long totalNumberOfPages;

static void* addressSpace;
static size_t addressSpaceLength;

static void handle_page_fault(int sig, siginfo_t *si, void *unused)
{
    long faultAddress = (long) si->si_addr;
    long faultingPage = (faultAddress - (long) addressSpace) / getpagesize() ; //0-based

    if (faultingPage > -1 && faultingPage < totalNumberOfPages) {
        ClientPageEntry* page = &(pages[faultingPage]);
        // TODO How dow we know if it was a read or a write? handle every possible case

    } else {
        // TODO how we should pass this fault to the OS?
    }
}

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
        free(networkInfo);
        free(request);
        free(response);
        return -1;
    }

    *nodes = response->numberOfNodes;
    *nid = response->assignedNodeId;
    nodeId = response->assignedNodeId;

    totalNumberOfPages = response->numberOfPages;
    pages = (ClientPageEntry*) malloc(sizeof(ClientPageEntry) * totalNumberOfPages);
    free(request);
    free(response);

    int i;
    for (i = 0; i < totalNumberOfPages; i++)
        pages[i].present = false;

    addressSpaceLength = (size_t) (getpagesize() * totalNumberOfPages);
    addressSpace = mmap(NULL, addressSpaceLength, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, -1);

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handle_page_fault;
    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        free(networkInfo);
        free(pages);
        return -1;
    }

    if (mprotect(addressSpace, addressSpaceLength, PROT_NONE) == -1) {
        free(networkInfo);
        free(pages);
        return -1;
    }
    return 0;
}

int DSM_node_exit(void) {

    NodeExitRequest* request = (NodeExitRequest*) malloc(sizeof(NodeExitRequest));
    request->nodeId = nodeId;
    NodeExitResponse* response = client_request_node_exit(request);

    int errorCode = response->errorCode;
    free(request);
    free(response);

    if (errorCode == 0) {
        // TODO Free Page table and everything else
        nodeId = 0;
        free(pages);
        return 0;
    }

    return -1;
}

void *DSM_alloc(size_t size) {

    AllocRequest* request = (AllocRequest*) malloc(sizeof(AllocRequest));
    request->nodeId = nodeId;
    request->size = size;
    AllocResponse* response = client_request_alloc(request);

    if (response->errorCode != 0) {
        free(request);
        free(response);
        return (void*)-1;
    }

    long address = response->address;
    free(request);
    free(response);

    void* resultingAddress = (void *) ((long)addressSpace + address);
    int pageSize = getpagesize();
    int numberOfPages = (int) ceil((double)size / pageSize);
    if (mprotect(resultingAddress, (size_t) (numberOfPages * pageSize), PROT_WRITE) == -1) {
        return (void*)-1;
    } else {
        return resultingAddress;
    }
}

void DSM_barrier(int barrier_id) {

}
