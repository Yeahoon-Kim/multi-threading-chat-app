#include "../chatProgram.h"

#define CLIENT_USAGE_ERROR_MSG "** Usage: ./chatClient [SERVER IP] [SERVER PORT] [USER NICKNAME]\n"
#define CLIENT_NICKNAME_LENGTH_ERROR_MSG "** Usage: User nickname less than 20\n"

#define CLIENT_CONNECT_ERROR_MSG "** Error while connect to server\n"

#define CLIENT_RECV_ERROR_MSG "** Client : Error while receive message from server\n"
#define CLIENT_SEND_ERROR_MSG "** Client : Error while send message to server\n"

extern int clientSocketDescriptor;
extern volatile int isRecvThreadTerminate;

void clientInterruptHandler(int signo);

void printUsageError();
void printUserNameLengthError();

int socketInit(struct sockaddr_in* sockAddr, char* IP, char *port);

int clientSocketSetting(char* IP, char* port);
int chattingClient(char* nickname);

void* threadReceive(void* params);