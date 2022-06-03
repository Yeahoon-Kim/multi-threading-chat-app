#include "chatClient.h"

/*
* Handle Keyboard Interrupt
* Use clientSocketDescriptor in global variable
*/
void clientInterruptHandler(int signo) {
    if(signo == SIGINT) {
        close(clientSocketDescriptor);
        putchar('\n');
        exit(EXIT_SUCCESS);
    }
}

/*
* Print Correct Usage
*/
void printUsageError() {
    perror(CLIENT_USAGE_ERROR_MSG);
}

/*
* Initialize socket
* Set socket's IP and port
* sockAddr : Socket struct
* IP       : String type IP
* port     : String type port
*/
int socketInit(struct sockaddr_in* sockAddr, char* IP, char *port) {
    errno = 0;
    char* endPtr = 0;
    in_addr_t IPNum;

    __uint16_t intPort = (__uint16_t)strtoul(port, &endPtr, 10);
    if(port == endPtr || errno == ERANGE || *endPtr) return FAILURE;

    IPNum = inet_addr(IP);
    if(IPNum == INADDR_NONE) return FAILURE; 

    sockAddr->sin_family = AF_INET;
    (sockAddr->sin_addr).s_addr = IPNum;
    sockAddr->sin_port = htons(intPort);

    return SUCCESS;
}

/*
* Print connection success message
*/
void printConnectionSuccessful() {
    puts(CLIENT_CONNECTION_MSG);
}

/*
* Setting socket
* clientSocketDescriptor : Client-side socket file descriptor
* IP                     : String type IP
* port                   : String type port number
*/
int clientSocketSetting(int* clientSocketDescriptor, char* IP, char* port) {
    struct sockaddr_in serverSocket = { 0 };

    *clientSocketDescriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(*clientSocketDescriptor < 0) {
        fputs(CREATE_SOCKET_ERROR_MSG, stderr);
        return SETTING_FAILURE;
    }

    if(socketInit(&serverSocket, IP, port) == FAILURE) {
        handleError(*clientSocketDescriptor, INIT_SOCKET_ERROR_MSG);
        return SETTING_FAILURE;
    }

    if(connect(*clientSocketDescriptor, (struct sockaddr*)&serverSocket, sizeof(serverSocket))) {
        handleError(*clientSocketDescriptor, CLIENT_CONNECT_ERROR_MSG);
        return SETTING_FAILURE;
    }

    printConnectionSuccessful();
    return SETTING_SUCCESS;
}

/*
* Chatting with connected server
* clientSocketDescriptor : Client-side socket file descriptor
*/
int chattingClient(int clientSocketDescriptor) {
    char inBuf[MAX_BUF] = { 0 }, outBuf[MAX_BUF] = { 0 };
    __int8_t flag;

    while( 1 ) {
        flag = sendMessage(clientSocketDescriptor, outBuf, MAX_BUF);
        if(flag == SEND_FAILURE) {
            handleError(clientSocketDescriptor, CLIENT_SEND_ERROR_MSG);
            return CHATTING_FAILURE;
        }

        else if(flag == SEND_END) {
            printf(DISCONNECTION_MSG);
            return CHATTING_SUCCESS;
        }

        flag = recvMessage(clientSocketDescriptor, inBuf, MAX_BUF);
        if(flag == RECV_FAILURE) {
            handleError(clientSocketDescriptor, CLIENT_RECV_ERROR_MSG);
            return CHATTING_FAILURE;
        }

        else if(flag == RECV_END) {
            printf(DISCONNECTION_MSG);
            return CHATTING_SUCCESS;
        }
    }
}