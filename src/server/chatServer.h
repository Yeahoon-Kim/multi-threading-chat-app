#include "../chatProgram.h"

#define BACKLOG 5
#define CLIENTNUM 5
#define EMPTYSOCKET -1
#define MAX_NICKNAME_LENGTH 20

#define SERVER_USAGE_ERROR_MSG "Usage: ./chatServer [PORT]\n"

#define SERVER_RECV_ERROR_MSG "** Server : Error while receive message from client\n"
#define SERVER_SEND_ERROR_MSG "** Server : Error while send message to client\n"

#define SERVER_CONNECTION_MSG "Connection from %s:%hu\n"

typedef struct clientBox {
    struct sockaddr_in clientSocket;
    int idx;
} CLIENTBOX;

typedef struct connectionSet {
    pthread_t tid;
    int sockfd;
} CONSET;

extern int serverSocketDescriptor;
extern volatile int connectedNum;
extern pthread_mutex_t mutex;
extern CONSET conset[CLIENTNUM];

void serverInterruptHandler(int signo);

void printUsageError();

int socketInit(struct sockaddr_in* sockAddr, char *port);

void printConnectionSuccessful(struct sockaddr_in *clientSocket);
int serverSocketSetting(int* serverSocketDescriptor, char* port);

void* threadConnection(void* params);

int broadcast(int fd, char* str, int isMirrored);