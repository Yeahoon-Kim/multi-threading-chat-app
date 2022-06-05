#include "chatServer.h"

/*
* Handle Keyboard Interrupt
* Close welcomming socket
* Send all of clients "QUIT" message and close all of connections with clients
* Use serverSocketDescriptor in global variable
*/
void serverInterruptHandler(const int signo) {
    if(signo == SIGINT) {
        if(close(serverSocketDescriptor)) {
            printf(CLOSE_ERROR_MSG);
            exit(EXIT_FAILURE);
        }
        
        broadcast(0, "QUIT\n", true);   // Send "QUIT" message to all of clients
        
#ifdef DEBUG
        printf("[*] Successfully broadcast \"QUIT\"\n");
#endif
        // Use conset variable which is global variable
        // Use mutex for prevent race condition vulnerability
        pthread_mutex_lock(&mutex);
        for(int i = 0; i < CLIENTNUM; i++) {
            if(conset[i].sockfd == EMPTYSOCKET) continue;

            if(close(conset[i].sockfd)) {
                printf(CLOSE_ERROR_MSG);
                exit(EXIT_FAILURE);
            }
        }
        pthread_mutex_unlock(&mutex);

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
int socketInit(struct sockaddr_in* sockAddr, const char *port) {
    errno = 0;
    char* endPtr = 0;

#ifdef DEBUG
    puts("[*] socketInit function");
#endif

    __uint16_t intPort = (__uint16_t)strtoul(port, &endPtr, 10);        // Change string type port to number
    if(port == endPtr or errno == ERANGE or *endPtr) return FAILURE;                      // Error detection

    sockAddr->sin_family = AF_INET;
    (sockAddr->sin_addr).s_addr = htonl(INADDR_ANY);                    // Change byte order to network order
    sockAddr->sin_port = htons(intPort);                                // Change byte order to network order

    return SUCCESS;
}

/*
* Print connection success message
* clientSocket : Client-side socket for printing IP and port number
*/
void printConnectionSuccessful(const struct sockaddr_in* clientSocket) {
    __uint16_t port = ntohs(clientSocket->sin_port);                    // Change byte order to host order
    char* IP = inet_ntoa(clientSocket->sin_addr);                       // Change IP type to string

    printf(SERVER_CONNECTION_MSG, IP, port);
}

/*
* Setting socket and return client-side socket descriptor
* serverSocketDescriptor : Server-side socket file descriptor
* port                   : String type port number
*/
int serverSocketSetting(int* serverSocketDescriptor, const char* port) {
    struct sockaddr_in serverSocket = { 0 };

#ifdef DEBUG
    puts("[*] serverSocketSetting function");
#endif

    // Create socket
    *serverSocketDescriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(*serverSocketDescriptor < 0) {
        fputs(CREATE_SOCKET_ERROR_MSG, stderr);
        return SETTING_FAILURE;
    }

    // Initialize socket
    if(socketInit(&serverSocket, port) == FAILURE) {
        handleError(*serverSocketDescriptor, INIT_SOCKET_ERROR_MSG, false);
        return SETTING_FAILURE;
    }

    // Bind a name to a server socket
    if(bind(*serverSocketDescriptor, (struct sockaddr *)&serverSocket, (socklen_t)sizeof(serverSocket))) {
        handleError(*serverSocketDescriptor, BIND_SOCKET_ERROR_MSG, false);
        return SETTING_FAILURE;
    }

    // Listen for connections on a socket
    if(listen(*serverSocketDescriptor, BACKLOG)) {
        handleError(*serverSocketDescriptor, LISTEN_ERROR_MSG, false);
        return SETTING_FAILURE;
    }

    return SETTING_SUCCESS;
}

/*
* Check connection and broadcast received data
* params : client box which has index and client socket address
*/
void* threadConnection(void* params) {
    char infoBuf[MAX_BUF] = { 0 }, msgBuf[MAX_BUF + 22] = { 0 }, nickName[MAX_NICKNAME_LENGTH] = { 0 };
    CLIENTBOX cb = *(CLIENTBOX *) params; // Copy client box to its stack
    __int8_t flag;

#ifdef DEBUG
    puts("[*] threadConnection function");
#endif

    // Print connection is successful to standard output
    printConnectionSuccessful(&cb.clientSocket);

    // Receive the client's nickname and broadcast including connected client
    recv(conset[cb.idx].sockfd, nickName, MAX_NICKNAME_LENGTH, 0);

#ifdef DEBUG
    printf("[*] User nickname: %s\n", nickName);
#endif

    snprintf(infoBuf, MAX_BUF, CONNECTION_MSG, nickName);
    broadcast(conset[cb.idx].sockfd, infoBuf, true);
    printf("%s", infoBuf);

    while( true ) {
        // Receive message from client
        flag = recvMessage(conset[cb.idx].sockfd, infoBuf, MAX_BUF);

#ifdef DEBUG
        printf("[*] Received flag: %d\n", flag);
#endif

        // Manage failure
        if(flag == RECV_FAILURE) {
            fputs(SERVER_RECV_ERROR_MSG, stderr);
            continue;
        }

        // Manage disconnection
        else if(flag == RECV_END) {
            // Broad disconnection message including disconnected client
            snprintf(infoBuf, MAX_BUF, DISCONNECTION_MSG, nickName);
            broadcast(conset[cb.idx].sockfd, infoBuf, true);
            puts(infoBuf);
            
            sendMessage(conset[cb.idx].sockfd, "QUIT\n", 5);

            // Prevent being closed socket from being used
            pthread_mutex_lock(&mutex);

            // Close its socket
            close(conset[cb.idx].sockfd);
            conset[cb.idx].sockfd = EMPTYSOCKET;
            connectedNum--;

            pthread_mutex_unlock(&mutex);

            break;
        }

        // Normal message
        snprintf(msgBuf, MAX_BUF + 22, "%s: %s", nickName, infoBuf);
        broadcast(conset[cb.idx].sockfd, msgBuf, false);

#ifdef DEBUG
        puts("[*] Successfully printed message");
        printf("[*] %s", msgBuf);
#endif
    }
}

/*
* Broadcast message to all connected clients
* fd         : sending client's file descriptor
* str        : message want to be broadcasted
* isMirrored : decide whether message is broadcasted including sending client
*/
int broadcast(const int fd, const char* str, const int isMirrored) {
    int i, strLen = strlen(str);
    __int8_t flag;

#ifdef DEBUG
    puts("[*] broadcast function");
    printf("[*] str: %s", str);
#endif

    // Prevent others from using socket while this function uses it
    pthread_mutex_lock(&mutex);

    for(i = 0; i < CLIENTNUM; i++) {
        if((conset[i].sockfd == fd and !isMirrored) or conset[i].sockfd == EMPTYSOCKET) continue;
        
#ifdef DEBUG
        printf("[*] Send message to index %d\n", i);
#endif

        flag = sendMessage(conset[i].sockfd, str, strLen);

        if(flag == SEND_FAILURE) {
#ifdef DEBUG
            puts("[*] Retransmission");
#endif
            // Retransmission
            i--;
            continue;
        }
    }

    pthread_mutex_unlock(&mutex);

    return CHATTING_SUCCESS;
}