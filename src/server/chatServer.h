#include "../chatProgram.h"

#define BACKLOG 5

#define SERVER_USAGE_ERROR_MSG "Usage: ./chatServer [PORT]\n"

#define SERVER_RECV_ERROR_MSG "** Server : Error while receive message from client\n"
#define SERVER_SEND_ERROR_MSG "** Server : Error while send message to client\n"

#define SERVER_CONNECTION_MSG "Connection from %s:%hu\n"

extern int serverSocketDescriptor;

void clientInterruptHandler(int signo);

void printUsageError();

int socketInit(struct sockaddr_in* sockAddr, char *port);

void printConnectionSuccessful(struct sockaddr_in *clientSocket);
int serverSocketSetting(int* serverSocketDescriptor, char* port);

int chattingServer(int serverSocketDescriptor, int clientSocketDescriptor);