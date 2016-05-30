//
// Created by andres on 28/05/16.
//

#ifndef DISTRIBUTED_SHARED_MEMORY_FILEREADER_H
#define DISTRIBUTED_SHARED_MEMORY_FILEREADER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char* clientIP;
    char* clientPort;
    char* clientUsername;
}ClientInfo;

typedef struct  {
    ClientInfo** clients;
    int size;
}ClientList;

ClientList* readFile(char* fileName);

#endif //DISTRIBUTED_SHARED_MEMORY_FILEREADER_H

