#include <stddef.h>
#include <stdlib.h>
#include <Requests.h>
#include <Responses.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "ClientHandle.h"
#include "Globals.h"
#include "ClientPageEntry.h"

PageResponse* client_handle_page_request(PageRequest *request) {
    PageResponse* response = (PageResponse*) malloc(sizeof(PageResponse));
    ClientPageEntry* page = &(pages[request->pageNumber]);
    if (!page->present) {
        response->errorCode = -5;
        return response;
    }
    int pageSize = getpagesize();
    if (request->ownershipOnly) {
        if (mprotect(addressSpace + request->pageNumber * pageSize, (size_t) pageSize, PROT_NONE) == -1) {
            response->errorCode = -6;
            fprintf(stderr, "%s\n", strerror(errno));
            return response;
        }
        page->ownership = 0;
        page->present = 0;
        page->isReadOnly = 1;
        response->errorCode = 0;
        return response;
    }

    int i;
    char* pageContents =  addressSpace + request->pageNumber * pageSize;
    for (i = 0; i < pageSize; i++)
        response->pageContents[i] = pageContents[i];
    if (request->readOnlyMode && !page->isReadOnly) {
        if (mprotect(addressSpace + request->pageNumber * pageSize, (size_t) pageSize, PROT_READ) == -1) {
            response->errorCode = -6;
            fprintf(stderr, "%s\n", strerror(errno));
            return response;
        }
        page->isReadOnly = 1;
    } else if (!request->readOnlyMode) {
        if (mprotect(addressSpace + request->pageNumber * pageSize, (size_t) pageSize, PROT_NONE) == -1) {
            response->errorCode = -6;
            fprintf(stderr, "%s\n", strerror(errno));
            return response;
        }
        page->isReadOnly = 1;
        page->ownership = 0;
        page->present = 0;
    }
    response->errorCode = 0;
    return response;
}

InvalidationResponse* client_handle_invalidation(InvalidationRequest *request) {
    InvalidationResponse* response = (InvalidationResponse*) malloc(sizeof(InvalidationResponse));
    ClientPageEntry* page = &(pages[request->pageNumber]);
    if (!page->present) {
        response->errorCode = -5;
        return response;
    }
    int pageSize = getpagesize();
    if (mprotect(addressSpace + request->pageNumber * pageSize, (size_t) pageSize, PROT_NONE) == -1) {
        response->errorCode = -6;
        fprintf(stderr, "%s\n", strerror(errno));
        return response;
    }

    page->present = false;
    page->ownership = false;
    page->isReadOnly = true;
    response->errorCode = 0;
    return response;
}
