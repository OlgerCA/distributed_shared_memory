#ifndef SERVER_H
#define SERVER_H

#define PORT 4400

int server_open(int sin_port, int max_conn);
void server_catch(int cx, int max_conn);
void server_multiplex(int clients[], int max_conn, int cx_max);
void server_attend(int cx);

#endif
