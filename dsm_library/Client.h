#ifndef CLIENT_H
#define CLIENT_H

static int server = -1;
static int client = -1;

int client_connect(char* s_addr, int sin_port);
int client_listen(int sin_port, int max_conn);
void client_listener(int e);
void client_attend(int cx);

#endif