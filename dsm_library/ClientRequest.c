#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <NetworkInfo.h>
#include <malloc.h>
#include "ClientRequest.h"

// This file should contain all socket related logic to communicate with server.
// Not sure if responses should be waited async.

static NetworkInfo* netInfo;
static int server = -1;

int open(char* s_addr, int sin_port) {
	struct sockaddr_in addr_server;

	int cx = socket(AF_INET, SOCK_STREAM, 0);
	if (cx == -1) {
		return cx;
	}
	
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = inet_addr(s_addr);
	addr_server.sin_port = htons(sin_port);

	int addr_server_size = sizeof(struct sockaddr);
	if (
		connect(cx, (struct sockaddr*) &addr_server, addr_server_size) == -1
	) {
		return -1;
	}
	
	return cx;
}

NodeInitResponse* client_request_node_init(NodeInitRequest* request, NetworkInfo* networkInfo) {
	netInfo = networkInfo;
	char* buffer1 = (char*) malloc(2);

	server = open(netInfo->serverName, netInfo->serverPort);
	char* message = (char*) malloc(MAXDATASIZE);
	
	sprintf(message, REQ_FORMAT, GET, INIT, ZERO, ZERO, ZERO, (long) ZERO);
	
	send(server, message, strlen(message), 0);
	recv(server, message, MAXDATASIZE, 0);
	
	NodeInitResponse* response = (NodeInitResponse*) malloc(sizeof(NodeInitResponse));

	sscanf(
		message,
		RES_FORMAT,
		&response->errorCode,
		&response->assignedNodeId,
		&response->numberOfNodes,
		&response->numberOfPages,
		buffer1
	);

	free(buffer1);
	free(message);

	return response;
}

NodeExitResponse* client_request_node_exit(NodeExitRequest* request) {
	char* message = (char*) malloc(MAXDATASIZE);
	int buffer1 = 0;
	int buffer2 = 0;
	long buffer3 = 0;
	char* buffer4 = (char*) malloc(2);
	
	sprintf(message, REQ_FORMAT, GET, EXIT, request->nodeId, ZERO, ZERO, (long) ZERO);
	
	send(server, message, strlen(message), 0);
	recv(server, message, MAXDATASIZE, 0);
	
	NodeExitResponse* response = (NodeExitResponse*) malloc(sizeof(NodeExitResponse));

	sscanf(
		message,
		RES_FORMAT,
		&response->errorCode,
		&buffer1,
		&buffer2,
		&buffer3,
		buffer4
	);
	
	free(buffer4);
	free(message);
	free(netInfo);

	return response;
}

AllocResponse* client_request_alloc(AllocRequest* request) {
	char* message = (char*) malloc(MAXDATASIZE);
	int buffer1 = 0;
	int buffer2 = 0;
	char* buffer3 = (char*) malloc(2);
	
	sprintf(message, REQ_FORMAT, GET, ALLO, request->nodeId, ZERO, ZERO, request->size);
	
	send(server, message, strlen(message), 0);
	recv(server, message, MAXDATASIZE, 0);
	
	AllocResponse* response = (AllocResponse*) malloc(sizeof(AllocResponse));

	sscanf(
		message,
		RES_FORMAT,
		&response->errorCode,
		&buffer1,
		&buffer2,
		&response->address,
		buffer3
	);
	
	free(buffer3);
	free(message);

	return response;
}

PageResponse* client_request_page(PageRequest* request) {
	char* message = (char*) malloc(MAXDATASIZE);
	int buffer1 = 0;
	int buffer2 = 0;
	long buffer3 = 0;
	
	sprintf(
		message,
		REQ_FORMAT,
		GET,
		PAGE,
		request->nodeId,
		request->ownershipOnly,
		request->readOnlyMode,
		request->pageNumber
	);
	
	send(server, message, strlen(message), 0);
	recv(server, message, MAXDATASIZE, 0);
	
	PageResponse* response = (PageResponse*) malloc(sizeof(PageResponse));

	sscanf(
		message,
		RES_FORMAT,
		&response->errorCode,
		&buffer1,
		&buffer2,
		&buffer3,
		response->pageContents
	);
	
	free(message);

	return response;
}

InvalidationResponse* client_request_invalidation(InvalidationRequest* request) {
	char* message = (char*) malloc(MAXDATASIZE);
	int buffer1 = 0;
	int buffer2 = 0;
	long buffer3 = 0;
	char* buffer4 = (char*) malloc(2);
	
	sprintf(message, REQ_FORMAT, GET, INVA, request->nodeId, ZERO, ZERO, request->pageNumber);
	
	send(server, message, strlen(message), 0);
	recv(server, message, MAXDATASIZE, 0);
	
	InvalidationResponse* response = (InvalidationResponse*) malloc(sizeof(InvalidationResponse));

	sscanf(
		message,
		RES_FORMAT,
		&response->errorCode,
		&buffer1,
		&buffer2,
		&buffer3,
		buffer4
	);
	
	free(buffer4);
	free(message);

	return response;
}

BarrierResponse* client_request_barrier(BarrierRequest* request) {
	char* message = (char*) malloc(MAXDATASIZE);
	int buffer1 = 0;
	int buffer2 = 0;
	long buffer3 = 0;
	char* buffer4 = (char*) malloc(2);
	
	sprintf(message, REQ_FORMAT, GET, BARR, request->nodeId, request->barrierId, ZERO, (long) ZERO);
	
	send(server, message, strlen(message), 0);
	recv(server, message, MAXDATASIZE, 0);
	
	BarrierResponse* response = (BarrierResponse*) malloc(sizeof(BarrierResponse));

	sscanf(
		message,
		RES_FORMAT,
		&response->errorCode,
		&buffer1,
		&buffer2,
		&buffer3,
		buffer4
	);
	
	free(buffer4);
	free(message);

	return response;
}