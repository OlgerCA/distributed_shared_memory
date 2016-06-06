#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <NetworkInfo.h>
#include <arpa/inet.h>
#include <errno.h>
#include <Requests.h>
#include <zconf.h>
#include "ServerHandle.h"
#include "Server.h"
#include "NetworkUtils.h"

int completedNodes;

int server_open(int sin_port, int max_conn) {
	struct sockaddr_in addr_server;
	completedNodes = 0;

	int option = 1;
	int cx = socket(AF_INET, SOCK_STREAM, 0);
	printf("Socket opened in server_open: %d\n", cx);
	if (cx == -1) {
		return cx;
	}
	setsockopt(cx, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option));
	addr_server.sin_family = AF_INET;
	// addr_server.sin_addr.s_addr = INADDR_ANY;
	addr_server.sin_addr.s_addr = inet_addr(getLocalIpAddress());
	addr_server.sin_port = htons(sin_port);

	int addr_server_size = sizeof(struct sockaddr_in);
	if (bind(cx, (struct sockaddr*) &addr_server, addr_server_size) == -1) {
		fprintf(stderr, "ERROR IN bind: %s\n", strerror(errno));
		return -1;
	}

	if (listen(cx, max_conn)) {
		fprintf(stderr, "ERROR IN listen: %s\n", strerror(errno));
		return -1;
	}

	return cx;
}

int server_connect(char* s_addr, int sin_port) {
	struct sockaddr_in addr_server;

	int option = 1;
	int cx = socket(AF_INET, SOCK_STREAM, 0);
	printf("Socket opened in server_connect: %d\n", cx);
	if (cx == -1) {
		fprintf(stderr, "%s\n", strerror(errno));
		return cx;
	}
	setsockopt(cx, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option));
	
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = inet_addr(s_addr);
	addr_server.sin_port = htons(sin_port);

	int addr_server_size = sizeof(struct sockaddr);
	if (connect(cx, (struct sockaddr*) &addr_server, addr_server_size) == -1) {
		fprintf(stderr, "ERROR CONECTING TO PORT %d: and addresss: %s,  %s\n", sin_port, s_addr, strerror(errno));
		return -1;
	}
	
	return cx;
}

void server_catch(int mainServerSocket, int numClients) {
	int clients[numClients];
	
	struct sockaddr addr_client;
	unsigned int addr_client_size = sizeof(struct sockaddr);
	int cx_new;
	int cx_max = -1;
	
	int i;
	for (i=0; i<numClients; i++) {
		cx_new = accept(mainServerSocket, &addr_client, &addr_client_size);
		printf("Socket opened in accept de server_catch: %d \n", cx_new);
		if (cx_new > cx_max) {
			cx_max = cx_new;
		}
		clients[i] = cx_new;

		server_attend(cx_new);
	}
	
	server_multiplex(mainServerSocket, clients, numClients, cx_max);
}

void server_multiplex(int cx, int clients[], int numClients, int cx_max) {
	fd_set writers;
	
	int i;
	do {
		FD_ZERO(&writers);
		for (i=0; i<numClients; i++) {
			if (clients[i] == -1) {
				continue;
			}
			FD_SET(clients[i], &writers);
		}
		
		select(cx_max+1, &writers, NULL, NULL, NULL);
		for (i=0; i<numClients; i++) {
			if (FD_ISSET(clients[i], &writers)) {
				// sleep(2);
				server_attend(clients[i]);
			}
		}
	} while (completedNodes < numClients);

	printf("Closing socket: %d \n", cx);
	shutdown(cx, SHUT_RDWR);
	close(cx);
}

void server_attend(int reqClientSocket) {
	char message[MAXDATASIZE];
	recv(reqClientSocket, message, MAXDATASIZE, 0);
	
	char ipAddress[20];
	char action[4];
	int param1 = 0;
	int param2 = 0;
	int param3 = 0;
	long param4 = 0;
	sscanf(message, REQ_FORMAT, ipAddress, action, &param1, &param2, &param3, &param4);
	
	/*if (strcmp(ipAddress, GET) != 0) {
		return;
	}*/
	// printf("recv completed: %d, %s\n", cx, message);
	if (strcmp(action, INIT) == 0) {
		NodeInitRequest* request = (NodeInitRequest*) malloc(sizeof(NodeInitRequest));
		strcpy(request->forwardAddress, ipAddress);
		request->forwardPort = param1;
		NodeInitResponse* response = server_handle_node_init(request, reqClientSocket);

		sprintf(
			message,
			RES_FORMAT,
			response->errorCode,
			response->assignedNodeId,
			response->numberOfNodes,
			response->numberOfPages,
			VOID
		);
		
		send(reqClientSocket, message, strlen(message), 0);
		
		free(request);
	}
	if (strcmp(action, EXIT) == 0) {
		NodeExitRequest* request = (NodeExitRequest*) malloc(sizeof(NodeExitRequest));
		request->nodeId = param1;
		NodeExitResponse* response = server_handle_node_exit(request);

		sprintf(
			message,
			RES_FORMAT,
			response->errorCode,
			ZERO,
			ZERO,
			(long) ZERO,
			VOID
		);
		
		send(reqClientSocket, message, strlen(message), 0);
		
		free(request);
		free(response);
		completedNodes++;
	}
	if (strcmp(action, ALLO) == 0) {
		AllocRequest* request = (AllocRequest*) malloc(sizeof(AllocRequest));
		request->nodeId = param1;
		request->size = param4;
		AllocResponse* response = server_handle_alloc(request);

		sprintf(
			message,
			RES_FORMAT,
			response->errorCode,
			response->servedFromCache,
			ZERO,
			response->address,
			VOID
		);
		
		send(reqClientSocket, message, strlen(message), 0);
		
		free(request);
		free(response);
	}
	if (strcmp(action, PAGE) == 0) {
		PageRequest* request = (PageRequest*) malloc(sizeof(PageRequest));
		request->nodeId = param1;
		request->ownershipOnly = param2;
		request->readOnlyMode = param3;
		request->pageNumber = param4;
		PageResponse* response = NULL;

		pageRequest:
		response = server_handle_page_request(reqClientSocket, request);

		sprintf(
			message,
			RES_FORMAT,
			response->errorCode,
			ZERO,
			ZERO,
			(long) ZERO,
			"&"
		);

		if(response->errorCode == 0) {
			size_t requestSize = strlen(message);
			if (!request->ownershipOnly) {
				char *contentBeforePage = strchr(message, '&');
				memcpy(contentBeforePage + 1, response->pageContents, getpagesize());
				requestSize = contentBeforePage - message + getpagesize() + 1;
			}

			send(reqClientSocket, message, requestSize, 0);
			server_handler_updateOwnerInfo(request);

		}else{
			goto pageRequest;
		}

		free(request);
		free(response);
	}
	if (strcmp(action, INVA) == 0) {
		InvalidationRequest* request = (InvalidationRequest*) malloc(sizeof(InvalidationRequest));
		request->nodeId = param1;
		request->pageNumber = param4;
		InvalidationResponse* response = server_handle_invalidation(request);

		sprintf(
			message,
			RES_FORMAT,
			response->errorCode,
			ZERO,
			ZERO,
			(long) ZERO,
			VOID
		);
		
		send(reqClientSocket, message, strlen(message), 0);
		
		free(request);
		free(response);
	}
	if (strcmp(action, BARR) == 0) {
		BarrierRequest* request = (BarrierRequest*) malloc(sizeof(BarrierRequest));
		request->nodeId = param1;
		request->barrierId = param2;
		server_handle_barrier(request);
		
		free(request);
	}
}
