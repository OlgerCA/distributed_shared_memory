#ifndef SERVER_H
#define SERVER_H

#define PORT 4400

extern int completedNodes;
int server_open(int sin_port, int max_conn);
int server_connect(char* s_addr, int sin_port);
void server_catch(int cx, int max_conn);
void server_multiplex(int clients[], int max_conn, int cx_max);
void server_attend(int cx);

#endif
