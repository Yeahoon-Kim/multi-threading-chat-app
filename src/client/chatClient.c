#include "chatClient.h"

/*
* Handle Keyboard Interrupt
* Use clientSocketDescriptor in global variable
*/
void clientInterruptHandler(const int signo) {
    if(signo == SIGINT) {
        sendMessage(clientSocketDescriptor, "QUIT\n", 5);

        putchar('\n');
        exit(EXIT_SUCCESS);
    }
}

/*
* Print Correct Usage
*/
void printUsageError() {
    fputs(CLIENT_USAGE_ERROR_MSG, stderr);
}

/*
* Print User nickname length
*/
void printUserNameLengthError() {
    fputs(CLIENT_NICKNAME_LENGTH_ERROR_MSG, stderr);
}

/*
* Initialize socket
* Set socket's IP and port
* sockAddr : Socket struct
* IP       : String type IP
* port     : String type port
*/
int socketInit(struct sockaddr_in* sockAddr, const char* IP, const char *port) {
    errno = 0;
    char* endPtr = 0;
    in_addr_t IPNum;

#ifdef DEBUG
    puts("[*] socketInit function");
#endif

    __uint16_t intPort = (__uint16_t)strtoul(port, &endPtr, 10);
    if(port == endPtr or errno == ERANGE or *endPtr) return FAILURE;

    IPNum = inet_addr(IP);
    if(IPNum == INADDR_NONE) return FAILURE; 

    sockAddr->sin_family = AF_INET;
    (sockAddr->sin_addr).s_addr = IPNum;
    sockAddr->sin_port = htons(intPort);

    return SUCCESS;
}

/*
* Setting socket
* clientSocketDescriptor : Client-side socket file descriptor
* IP                     : String type IP
* port                   : String type port number 
*/
int clientSocketSetting(const char* IP, const char* port) {
    struct sockaddr_in serverSocket = { 0 };

#ifdef DEBUG
    puts("[*] clientSocketSetting function");
#endif

    clientSocketDescriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocketDescriptor < 0) {
        fputs(CREATE_SOCKET_ERROR_MSG, stderr);
        return SETTING_FAILURE;
    }

    if(socketInit(&serverSocket, IP, port) == FAILURE) {
        handleError(clientSocketDescriptor, INIT_SOCKET_ERROR_MSG, false);
        return SETTING_FAILURE;
    }

    if(connect(clientSocketDescriptor, (struct sockaddr*)&serverSocket, sizeof(serverSocket))) {
        handleError(clientSocketDescriptor, CLIENT_CONNECT_ERROR_MSG, false);
        return SETTING_FAILURE;
    }

    return SETTING_SUCCESS;
}

/*
* Receive message from server and print it to its standard output
* params : User nickname
*/
void* threadReceive(void* params) {
    char recvBuf[MAX_BUF] = { 0 }, closedMessage[MAX_BUF] = { 0 };
    __int8_t flag;

#ifdef DEBUG
    puts("[*] threadReceive function");
#endif
    // Make disconnection message
    snprintf(closedMessage, MAX_BUF, DISCONNECTION_MSG, (char *)params);

    while( true ) {
#ifdef DEBUG
        sleep(1);
        puts("[*] Before receive message from server");
#endif
        // Receive message from server and save
        flag = recvMessage(clientSocketDescriptor, recvBuf, MAX_BUF);

#ifdef DEBUG
        printf("[*] Received flag: %d\n", flag);
#endif
        // In case of failure
        if(flag == RECV_FAILURE) {
            fputs(CLIENT_RECV_ERROR_MSG, stderr);
            continue;
        }
        // In case of end
        else if(flag == RECV_END) break;

        // In case of normal message
        else printf("%s", recvBuf);
#ifdef DEBUG
        puts("[*] Successfully receive message from server");
#endif
    } // end of while loop

#ifdef DEBUG
        puts("[*] Successfully escape from loop in threadReceive");
#endif

    // Change state variable
    isRecvThreadTerminate = true;
    pthread_exit(CHATTING_SUCCESS);
}

/*
* Chatting with connected server
* nickname : User nickname
*/
int chattingClient(const char* nickname) {
    char sendBuf[MAX_BUF] = { 0 };
    pthread_t tid;
    __int8_t flag;

#ifdef DEBUG
    puts("[*] chattingClient function");
#endif
    // Initialize state variable
    isRecvThreadTerminate = false;

    // Make thread for receive message from server
    pthread_create(&tid, NULL, (void*(*)(void *))threadReceive, (void*)nickname);

    // Send user nickname to server
    sendMessage(clientSocketDescriptor, nickname, strlen(nickname));

#ifdef DEBUG
    printf("[*] Successfully send nickname: %s\n", nickname);
#endif

    while( true ) {
        // Check if threadReceive function is terminated and escape loop
        if(isRecvThreadTerminate == true) {
            puts("** Server Terminated!");
            break;
        }

        // Receive message from user and send it to server 
        flag = scanAndSendMessage(clientSocketDescriptor, sendBuf, MAX_BUF);

#ifdef DEBUG
        printf("[*] Scanned string: %s", sendBuf);
#endif
        // In case of failure
        if(flag == SEND_FAILURE) {
            handleError(clientSocketDescriptor, CLIENT_SEND_ERROR_MSG, true);
            return CHATTING_FAILURE;
        }

        // In case of end
        else if(flag == SEND_END) {
            pthread_join(tid, NULL);
            break;
        }
    } // end of while loop

#ifdef DEBUG
    puts("[*] Successfully escape from loop in threadReceive");
#endif

    return CHATTING_SUCCESS;
}