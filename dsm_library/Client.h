#ifndef CLIENT_H
#define CLIENT_H

extern int socketToServer;
extern int clientSocket;
extern int isNodeRunning;

int client_connect(char* s_addr, int sin_port);
int client_listen(int sin_port, int max_conn);
void client_listener(int e);
void client_attend(int cx);
void client_closeSockets();
#endif
