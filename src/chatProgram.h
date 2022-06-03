#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <sys/types.h>

#define CREATE_SOCKET_ERROR_MSG "** Error while create socket\n"
#define INIT_SOCKET_ERROR_MSG "** Error while initialize socket\n"
#define BIND_SOCKET_ERROR_MSG "** Error while socket binding\n"
#define LISTEN_ERROR_MSG "** Error while listen for connections on a socket\n"
#define ACCEPT_ERROR_MSG "** Error while accept a connections on a socket\n"
#define CLOSE_ERROR_MSG "** Error while closing socket\n"

#define DISCONNECTION_MSG "Disconnected\n"

#define MAX_BUF 1024

#define SUCCESS 0
#define FAILURE -1

#define RECV_SUCCESS 1
#define RECV_FAILURE -1
#define RECV_END 0

#define SEND_SUCCESS 1
#define SEND_FAILURE -1
#define SEND_END 0

#define CHATTING_SUCCESS 0
#define CHATTING_FAILURE -1

#define SETTING_SUCCESS 0
#define SETTING_FAILURE -1

void handleError(int fd, const char * msg);

int recvMessage(int sockfd, char* buffer, int bufLen);
int sendMessage(int sockfd, char* buffer, int bufLen);