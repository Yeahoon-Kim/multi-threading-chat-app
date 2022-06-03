#include "../chatProgram.h"

#define CLIENT_USAGE_ERROR_MSG "** Usage: ./chatClient [SERVER IP] [SERVER PORT]\n"

#define CLIENT_CONNECT_ERROR_MSG "** Error while connect to server\n"

#define CLIENT_RECV_ERROR_MSG "** Client : Error while receive message from server\n"
#define CLIENT_SEND_ERROR_MSG "** Client : Error while send message to server\n"

#define CLIENT_CONNECTION_MSG "Connected"

extern int clientSocketDescriptor;

void clientInterruptHandler(int signo);

void printUsageError();

int socketInit(struct sockaddr_in* sockAddr, char* IP, char *port);

int clientSocketSetting(int* clientSocketDescriptor, char* IP, char* port);
int chattingClient(int clientSocketDescriptor);