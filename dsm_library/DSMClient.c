#include <sys/mman.h>
#include <Requests.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Responses.h>
#include <NetworkInfo.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "Globals.h"
#include "DSMClient.h"
#include "ClientRequest.h"
#include "ClientPageEntry.h"

// This file should contain all memory protection and mapping logic.
// This is the only API used by client applications.

int nodeId;

ClientPageEntry* pages;
long totalNumberOfPages;

void* addressSpace;
size_t addressSpaceLength;


int DSM_node_copy_page_contents(int faultingPage, int pageSize, PageResponse response);
PageResponse response;

static void handle_page_fault(int sig, siginfo_t *si, void *unused)
{
    long faultAddress = (long) si->si_addr;
    int pageSize = getpagesize();
    long faultingPage = (faultAddress - (long) addressSpace) / pageSize; //0-based

    if (faultingPage > -1 && faultingPage < totalNumberOfPages) {
        ClientPageEntry* page = &(pages[faultingPage]);
        // TODO How do we know if it was a read or a write?
        bool read = false;
        PageRequest pageRequest;

        pageRequest.nodeId = nodeId;
        pageRequest.pageNumber = faultingPage;

        // page present scenarios, no read-faults happen if the page is present, only write-faults
        if(page->present){
            // if I am the owner, the fault was caused by a write operation on a page that has copies
            if (page->ownership) {
                InvalidationRequest invalidationRequest;
                InvalidationResponse* invalidationResponse;
                int errorCode;
                invalidationRequest.nodeId = nodeId;
                invalidationRequest.pageNumber = faultingPage;
                invalidationResponse = client_request_invalidation(&invalidationRequest);
                errorCode = invalidationResponse->errorCode;
                free(invalidationResponse);
                if (errorCode == 0) {
                    if (mprotect(addressSpace + faultingPage * pageSize, (size_t) pageSize, PROT_WRITE | PROT_READ) == -1)
                        fprintf(stderr, "%s\n", strerror(errno));
                    else
                        page->isReadOnly = 0;
                }
                return;
            }else{ // if I am not the owner, the fault was caused by a write operation on a page which is not mine
                pageRequest.ownershipOnly = 1; // in such case, I will be the new owner
            }
        }else{ // if page not present it must be requested
            pageRequest.ownershipOnly = 0;
            pageRequest.readOnlyMode = 1; // this works, by default saying that the page will be read only
            // if it should have been with write privileges then another fault will happen that will update the access
        }

        response =  client_request_page(&pageRequest);
        if (response.errorCode != 0) {
            return;
        }

        // just requesting ownership, the page was not requested
        if (pageRequest.ownershipOnly) {
            if (mprotect(addressSpace + faultingPage * pageSize, (size_t) pageSize, PROT_WRITE | PROT_READ) == -1) {
                //free(response);
                fprintf(stderr, "%s\n", strerror(errno));
            } else {
                page->isReadOnly = 0;
                page->ownership = 1;
            }
            return;
        }

        // the page was requested, so its contents must be copied locally
        if(!DSM_node_copy_page_contents(faultingPage, pageSize, response)){
            return;
        }

        // the page was requested in read only mode
        if (pageRequest.readOnlyMode) {
            if (mprotect(addressSpace + faultingPage * pageSize, (size_t) pageSize, PROT_READ) == -1) {
                //free(response);
                fprintf(stderr, "%s\n", strerror(errno));
                return;
            }
            page->isReadOnly = 1;
            page->ownership = 0;
        } else { // the page was requested for me to be the new owner
            if (mprotect(addressSpace + faultingPage * pageSize, (size_t) pageSize, PROT_WRITE | PROT_READ) == -1) {
                //free(response);
                fprintf(stderr, "%s\n", strerror(errno));
                return;
            }
            page->isReadOnly = 0;
            page->ownership = 1;
        }
        page->present = 1;
    }
}

// sets the contents of the page that the response returned to the local faulting page
int DSM_node_copy_page_contents(int faultingPage, int pageSize, PageResponse response){
    // sets the protection of the faulting page to read/write, to copy the contents
    if (mprotect(addressSpace + faultingPage * pageSize, (size_t) pageSize, PROT_WRITE | PROT_READ) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 0;
    }
    char* startingPageAddress = addressSpace + faultingPage * pageSize;
    int i = 0;
    for (; i < pageSize; i++)
        startingPageAddress[i] = response.pageContents[i];
    //probably with a memcopy should work too

    return 1;
}

// From arguments we should receive server ip address and port, and also the client port to use.
int DSM_node_init(int *argc, char ***argv, int *nodes, int *nid) {

    if (*argc <= 3)
        return -1;

    NetworkInfo* networkInfo = (NetworkInfo*) malloc(sizeof(NetworkInfo));
    networkInfo->serverName = (*argv)[1];
    networkInfo->serverPort = atoi((*argv)[2]);
    networkInfo->clientPort = atoi((*argv)[3]);

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
    for (i = 0; i < totalNumberOfPages; i++) {
        pages[i].present = false;
        pages[i].ownership = false;
        pages[i].isReadOnly = true;
    }

    addressSpaceLength = (size_t) (getpagesize() * totalNumberOfPages);
    addressSpace = mmap(NULL, addressSpaceLength, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addressSpace == MAP_FAILED) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -1;
    }

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
        fprintf(stderr, "%s\n", strerror(errno));
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
        munmap(addressSpace, addressSpaceLength);
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
    void* resultingAddress = (void *) ((long)addressSpace + address);
    bool servedFromCache = response->servedFromCache;
    free(request);
    free(response);

    if (servedFromCache)
        return resultingAddress;

    int pageSize = getpagesize();
    int numberOfPages = (int) ceil((double)size / pageSize);
    if (mprotect(resultingAddress, (size_t) (numberOfPages * pageSize), PROT_WRITE | PROT_READ) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return (void*)-1;
    } else {
        long i = 0;
        long pageNumber = address / pageSize;
        for (i = 0; i < numberOfPages; i++) {
            pages[pageNumber + i].present = true;
            pages[pageNumber + i].ownership = true;
            pages[pageNumber + i].isReadOnly = false;
        }
        return resultingAddress;
    }
}

void DSM_barrier(int barrier_id) {
    BarrierRequest* request = (BarrierRequest *)malloc(sizeof(BarrierRequest));

    request->barrierId = barrier_id;
    request->nodeId = nodeId;

    client_request_barrier(request);

    return;
}
