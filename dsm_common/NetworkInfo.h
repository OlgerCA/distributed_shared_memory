
#ifndef DISTRIBUTED_SHARED_MEMORY_NETWORKINFO_H
#define DISTRIBUTED_SHARED_MEMORY_NETWORKINFO_H

#define GET "GET"
#define INIT "INIT"
#define EXIT "EXIT"
#define ALLO "ALLO"
#define PAGE "PAGE"
#define INVA "INVA"
#define BARR "BARR"
#define VOID ""
#define ZERO 0

#define REQ_FORMAT "%s %s %d %d %d %ld"
#define RES_FORMAT "%d %d %d %ld %s"

#define MAXCONN 8
#define MAXDATASIZE 32668

typedef struct {
    char* serverName;
    int serverPort;
    char* clientForwardIp;
    int clientForwardPort;
} NetworkInfo;

#endif