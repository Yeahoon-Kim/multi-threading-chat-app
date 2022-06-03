#include "chatServer.h"

CONSET conset[CLIENTNUM];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int serverSocketDescriptor;
volatile int connectedNum;

/*
* Socket 통신을 위한 Server 프로그램 작성
* Server : socket() -> bind() -> listen() -> accept() -> send() -> recv()
* Client : socket() ->                connect() ->       recv() -> send() -> close()
*/
int main(int argc, char *argv[]) {
    int clientSocketDescriptor, i;
    struct sockaddr_in clientSocket = { 0 };
    socklen_t clientSocketLength = sizeof(clientSocket);
    CLIENTBOX cliBox;

    signal(SIGINT, serverInterruptHandler);

    // The server MUST take one command-line argument as a port number for the program
    if(argc != 2) {
        printUsageError();
        exit(EXIT_FAILURE);
    }

    // Initialize all of file descriptors to -1
    for(i = 0; i < CLIENTNUM; i++) conset[i].sockfd = EMPTYSOCKET;
    
    // socket setting
    if(serverSocketSetting(&serverSocketDescriptor, argv[1]) == SETTING_FAILURE) exit(EXIT_FAILURE);

    // Connect to client and make thread
    while( true ) {
        // Accept
        clientSocketDescriptor = accept(serverSocketDescriptor, (struct sockaddr*)&clientSocket, &clientSocketLength);

        // Check if accept is successful and if there exist any empty sockets available
        if(clientSocketDescriptor < 0 or connectedNum >= CLIENTNUM) {
            handleError(clientSocketDescriptor, ACCEPT_ERROR_MSG);
            continue;
        }

        pthread_mutex_lock(&mutex);
        for(i = 0; i < CLIENTNUM; i++) {
            if(conset[i].sockfd != EMPTYSOCKET) continue;

            conset[i].sockfd = clientSocketDescriptor;
            connectedNum++;
            cliBox.idx = i;
            cliBox.clientSocket = clientSocket;

            break;
        }
        pthread_mutex_unlock(&mutex);

        // make thread to communicate with client
        if(pthread_create(&conset[cliBox.idx].tid, NULL, (void*(*)(void*))threadConnection, (void*)&cliBox) != 0) {
            puts(PTHREAD_CREATE_ERROR_MSG);

            close(clientSocketDescriptor);
            conset[cliBox.idx].sockfd = EMPTYSOCKET;
            connectedNum--;
        }
    }

    return 0;
}