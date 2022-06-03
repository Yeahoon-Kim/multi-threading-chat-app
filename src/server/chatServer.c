#include "chatServer.h"

/*
* Handle Keyboard Interrupt
* Use serverSocketDescriptor in global variable
*/
void clientInterruptHandler(int signo) {
    if(signo == SIGINT) {
        close(serverSocketDescriptor);
        puts("");
        exit(EXIT_SUCCESS);
    }
}

/*
* Print Correct Usage
*/
void printUsageError() {
    fputs(SERVER_USAGE_ERROR_MSG, stderr);
}

/*
* Initialize socket
* Set socket's port
* sockAddr : Socket struct
* port     : String type port
*/
int socketInit(struct sockaddr_in* sockAddr, char *port) {
    errno = 0;
    char* endPtr = 0;

    __uint16_t intPort = (__uint16_t)strtoul(port, &endPtr, 10);        // Change string type port to number
    if(port == endPtr || errno == ERANGE || *endPtr) return FAILURE;                      // Error detection

    sockAddr->sin_family = AF_INET;
    (sockAddr->sin_addr).s_addr = htonl(INADDR_ANY);                    // Change byte order to network order
    sockAddr->sin_port = htons(intPort);                                // Change byte order to network order

    return SUCCESS;
}

/*
* Print connection success message
* clientSocket : Client-side socket for printing IP and port number
*/
void printConnectionSuccessful(struct sockaddr_in* clientSocket) {
    __uint16_t port = ntohs(clientSocket->sin_port);                    // Change byte order to host order
    char* IP = inet_ntoa(clientSocket->sin_addr);                       // Change IP type to string

    printf(SERVER_CONNECTION_MSG, IP, port);
}

/*
* Setting socket and return client-side socket descriptor
* serverSocketDescriptor : Server-side socket file descriptor
* port                   : String type port number
*/
int serverSocketSetting(int* serverSocketDescriptor, char* port) {
    struct sockaddr_in serverSocket = { 0 }, clientSocket = { 0 };
    socklen_t clientSocketLength = sizeof(clientSocket);
    int clientSocketDescriptor;

    // Create socket
    *serverSocketDescriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(*serverSocketDescriptor < 0) {
        fputs(CREATE_SOCKET_ERROR_MSG, stderr);
        return SETTING_FAILURE;
    }

    // Initialize socket
    if(socketInit(&serverSocket, port) == FAILURE) {
        handleError(*serverSocketDescriptor, INIT_SOCKET_ERROR_MSG);
        return SETTING_FAILURE;
    }

    // Bind a name to a server socket
    if(bind(*serverSocketDescriptor, (struct sockaddr *)&serverSocket, (socklen_t)sizeof(serverSocket))) {
        handleError(*serverSocketDescriptor, BIND_SOCKET_ERROR_MSG);
        return SETTING_FAILURE;
    }

    // Listen for connections on a socket
    if(listen(*serverSocketDescriptor, BACKLOG)) {
        handleError(*serverSocketDescriptor, LISTEN_ERROR_MSG);
        return SETTING_FAILURE;
    }

    // Accept
    clientSocketDescriptor = accept(*serverSocketDescriptor, (struct sockaddr*)&clientSocket, &clientSocketLength);
    if(clientSocketDescriptor < 0) {
        handleError(*serverSocketDescriptor, ACCEPT_ERROR_MSG);
        return SETTING_FAILURE;
    }

    printConnectionSuccessful(&clientSocket);
    return clientSocketDescriptor;
}

/*
* Chatting with connected client
* serverSocketDescriptor : Server-side socket file descriptor
* clientSocketDescriptor : Client-side socket file descriptor
*/
int chattingServer(int serverSocketDescriptor, int clientSocketDescriptor) {
    char inBuf[MAX_BUF] = { 0 }, outBuf[MAX_BUF] = { 0 };
    __int8_t flag;

    while( 1 ) {
        flag = recvMessage(clientSocketDescriptor, inBuf, MAX_BUF);
        if(flag == RECV_FAILURE) {
            handleError(serverSocketDescriptor, SERVER_RECV_ERROR_MSG);
            return CHATTING_FAILURE;
        }

        else if(flag == RECV_END) {
            printf(DISCONNECTION_MSG);
            return CHATTING_SUCCESS;
        }

        flag = sendMessage(clientSocketDescriptor, outBuf, MAX_BUF);
        if(flag == SEND_FAILURE) {
            handleError(serverSocketDescriptor, SERVER_SEND_ERROR_MSG);
            return CHATTING_FAILURE;
        }

        else if(flag == SEND_END) {
            printf(DISCONNECTION_MSG);
            return CHATTING_SUCCESS;
        }
    }
}