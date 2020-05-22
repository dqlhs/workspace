#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <asm-generic/socket.h>
#include <semaphore.h>


#define SIZE 512


void sock_close(int socket);

int create_socket(void);

int sock_bind(int lisfd, int port);

int sock_listen(int lisfd, int max_con);

int sock_accept(int lisfd);

int sock_connect(int clifd, char *ipaddr, int port);

int sock_send(int sockfd, void *buf, size_t len, int flags);

int sock_recv(int sockfd, void *buf, size_t len, int flags);

void socket_recv(int* socketrev);

void socket_server_init(int* port);

int socket_client_init(char *ipadd,int port);



