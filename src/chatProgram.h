#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
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

#define PTHREAD_CREATE_ERROR_MSG "** Error while create thread for chatting\n"

#define CONNECTION_MSG "%s is connected\n"
#define DISCONNECTION_MSG "%s is disconnected\n"

/* Fixed buffer size */
#define MAX_BUF 1024

/* Return value for socketInit function in both server and client */
#define SUCCESS 0
#define FAILURE -1

/* Return value for recvMessage */
#define RECV_SUCCESS 1
#define RECV_FAILURE -1
#define RECV_END 0

/* Return value for sendMessage and scanAndSendMessage function */
#define SEND_SUCCESS 1
#define SEND_FAILURE -1
#define SEND_END 0

/* Return value for chatting function in both server and client */
#define CHATTING_SUCCESS 0
#define CHATTING_FAILURE -1

/* Return value for setting function in both server and client */
#define SETTING_SUCCESS 0
#define SETTING_FAILURE -1

// Make code clear
#define true 1
#define false 0
#define and &&
#define or ||

// For debugging
#define DEBUG

void handleError(const int sockfd, const char * msg, const int sendQUIT);

int recvMessage(const int sockfd, char* buffer, const int bufLen);
int sendMessage(const int sockfd, const char* buffer, const int bufLen);

int scanAndSendMessage(const int sockfd, char* buffer, const int bufLen);