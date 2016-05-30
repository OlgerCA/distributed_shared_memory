#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <NetworkInfo.h>
#include <errno.h>
#include "ClientRequest.h"
#include "Client.h"

// This file should contain all socket related logic to communicate with server.
// Not sure if responses should be waited async.

static NetworkInfo* netInfo;

NodeInitResponse* client_request_node_init(NodeInitRequest* request, NetworkInfo* networkInfo) {
	netInfo = networkInfo;
	char* buffer1 = (char*) malloc(20);
	int buffer2 = 0;

	server = client_connect(netInfo->serverName, netInfo->serverPort);
	client = client_listen(netInfo->clientForwardPort, MAXCONN);
	char* message = (char*) malloc(MAXDATASIZE);
	
	sprintf(message, REQ_FORMAT, networkInfo->clientForwardIp, INIT, networkInfo->clientForwardPort, ZERO, ZERO, (long) ZERO);
	
	send(server, message, strlen(message) + 1, 0);
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
	
	send(server, message, strlen(message) + 1, 0);
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
	char* buffer2 = (char*) malloc(2);
	
	sprintf(message, REQ_FORMAT, GET, ALLO, request->nodeId, ZERO, ZERO, request->size);
	
	send(server, message, strlen(message) + 1, 0);
	recv(server, message, MAXDATASIZE, 0);
	
	AllocResponse* response = (AllocResponse*) malloc(sizeof(AllocResponse));

	sscanf(
		message,
		RES_FORMAT,
		&response->errorCode,
		(int*) &response->servedFromCache,
		&buffer1,
		&response->address,
		buffer2
	);
	
	free(buffer2);
	free(message);

	return response;
}

PageResponse client_request_page(PageRequest* request) {
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
	
	send(server, message, strlen(message) + 1, 0);
	recv(server, message, MAXDATASIZE, 0);
	
	PageResponse response;

	sscanf(
		message,
		RES_FORMAT,
		&response.errorCode,
		&buffer1,
		&buffer2,
		&buffer3,
		response.pageContents
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
	
	send(server, message, strlen(message) + 1, 0);
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
	
	send(server, message, strlen(message) + 1, 0);
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
