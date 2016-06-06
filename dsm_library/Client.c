#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <Responses.h>
#include "Client.h"
#include "ClientHandle.h"
#include "ClientRequest.h"

int socketToServer = -1;
int clientSocket = -1;
int isNodeRunning = 1;

int client_connect(char* s_addr, int sin_port) {
	struct sockaddr_in addr_server;

	int option = 1;
	int cx = socket(AF_INET, SOCK_STREAM, 0);
	printf("Socket opened in client_connect: %d\n", cx);
	if (cx == -1) {
		return cx;
	}

	setsockopt(cx, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option));
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = inet_addr(s_addr);
	addr_server.sin_port = htons(sin_port);

	int addr_server_size = sizeof(struct sockaddr);
	if (
		connect(cx, (struct sockaddr*) &addr_server, addr_server_size) == -1
	) {
		fprintf(stderr, "%s\n", strerror(errno));
		return -1;
	}
	
	return cx;
}

int client_listen(int sin_port, int max_conn) {
	signal(SIGIO, client_listener);
	
	struct sockaddr_in addr_server;
	int option = 1;
	int cx = socket(AF_INET, SOCK_STREAM, 0);
	printf("Socket opened in client_listen: %d\n", cx);
	if (cx == -1) {
		return cx;
	}

	setsockopt(cx, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option));
	//
	fcntl(cx, F_SETFL, O_NONBLOCK);
	fcntl(cx, F_SETFL, O_ASYNC);
	
	if (fcntl(cx, F_SETOWN, getpid()) < 0) {
		return -1;
	}
	//
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = INADDR_ANY;
	addr_server.sin_port = htons(sin_port);

	int addr_server_size = sizeof(struct sockaddr_in);
	if (bind(cx, (struct sockaddr*) &addr_server, addr_server_size) == -1) {
		return -1;
	}

	if (listen(cx, max_conn)) {
		return -1;
	}
	
	return cx;
}

void client_listener(int e) {
	if(isNodeRunning) {
		struct sockaddr addr_client;

		unsigned int addr_client_size = sizeof(struct sockaddr);
		int cx = accept(clientSocket, &addr_client, &addr_client_size);
		printf("Socket opened in accept of clientListener: %d\n", cx);
		if (cx == -1) {
			fprintf(stdout, "Error accepting client port,  %s\n", strerror(errno));
			return;
		}

		client_attend(cx);
	}
}

void client_attend(int cx) {
	char message [MAXDATASIZE];
	recv(cx, message, MAXDATASIZE, 0);
	
	char method[4];
	char action[4];
	int param1 = 0;
	int param2 = 0;
	int param3 = 0;
	long param4 = 0;
	sscanf(message, REQ_FORMAT, method, action, &param1, &param2, &param3, &param4);
	
	if (strcmp(method, GET) != 0) {
		return;
	}
	// printf("recv completed: %d, %s\n", cx, message);
	if (strcmp(action, PAGE) == 0) {
		PageRequest* request = (PageRequest*) malloc(sizeof(PageRequest));
		request->nodeId = param1;
		request->ownershipOnly = param2;
		request->readOnlyMode = param3;
		request->pageNumber = param4;

		PageResponse* response = client_handle_page_request(request);

		sprintf(
			message,
			RES_FORMAT,
			response->errorCode,
			ZERO,
			ZERO,
			(long) ZERO,
			"&"
		);

		size_t requestSize = 0;

		if(!request->ownershipOnly) {
			char *contentBeforePage = strchr(message, '&');
			memcpy(contentBeforePage + 1, response->pageContents, getpagesize());
			requestSize = contentBeforePage - message + getpagesize() + 1;
		}else{
			requestSize = strlen(message);
		}

		send(cx, message, requestSize, 0);
		
		free(request);
		free(response);
	}
	if (strcmp(action, INVA) == 0) {
		InvalidationRequest* request = (InvalidationRequest*) malloc(sizeof(InvalidationRequest));
		request->nodeId = param1;
    	request->pageNumber = param4;
		InvalidationResponse* response = client_handle_invalidation(request);

		sprintf(
			message,
			RES_FORMAT,
			response->errorCode,
			ZERO,
			ZERO,
			request->pageNumber,
			VOID
		);
		
		send(cx, message, strlen(message), 0);
		
		free(request);
		free(response);
	}

	printf("Closing socket: %d \n", cx);
	shutdown(cx, SHUT_RDWR);
	close(cx);
}

void client_closeSockets(){
	printf("Closing socket: %d \n", socketToServer);
	int returnValue = shutdown(socketToServer, SHUT_RDWR);
	if(returnValue != 0){
		fprintf(stderr, "Error shutting down client port,  %s\n", strerror(errno));
	}
	returnValue = close(socketToServer);
	if(returnValue != 0){
		fprintf(stderr, "Error closing client port,  %s\n", strerror(errno));
	}

	printf("Closing socket: %d \n", clientSocket);
	returnValue = shutdown(clientSocket, SHUT_RDWR);
	if(returnValue != 0){
		fprintf(stderr, "Error shutting down server port,  %s\n", strerror(errno));
	}

	returnValue = 	close(clientSocket);
	if(returnValue != 0){
		fprintf(stderr, "Error closing server port,  %s\n", strerror(errno));
	}
}