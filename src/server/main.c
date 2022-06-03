#include "chatServer.h"

int serverSocketDescriptor;

/*
* Socket 통신을 위한 Server 프로그램 작성
* Server : socket() -> bind() -> listen() -> accept() -> send() -> recv()
* Client : socket() ->                connect() ->       recv() -> send() -> close()
*/
int main(int argc, char *argv[]) {
    int clientSocketDescriptor;

    signal(SIGINT, clientInterruptHandler);

    // The server MUST take one command-line argument as a port number for the program
    if(argc != 2) {
        printUsageError();
        exit(EXIT_FAILURE);
    }

    clientSocketDescriptor = serverSocketSetting(&serverSocketDescriptor, argv[1]);
    if(clientSocketDescriptor == SETTING_FAILURE) exit(EXIT_FAILURE);
    
    if(chattingServer(serverSocketDescriptor, clientSocketDescriptor)) {
        exit(EXIT_FAILURE);
    }

    if(close(serverSocketDescriptor)) {
        perror(CLOSE_ERROR_MSG);
        exit(EXIT_FAILURE);
    }

    return 0;
}