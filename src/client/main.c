#include "chatClient.h"

int clientSocketDescriptor;

/*
* Socket 통신을 위한 Client 프로그램 작성
* Server : socket() -> bind() -> listen() -> accept() -> send() -> recv()
* Client : socket() ->                connect() ->       recv() -> send() -> close()
*/
int main(int argc, char* argv[]) {
    signal(SIGINT, clientInterruptHandler);

    if(argc != 3) {
        printUsageError();
        exit(EXIT_FAILURE);
    }

    if(clientSocketSetting(&clientSocketDescriptor, argv[1], argv[2]) == SETTING_FAILURE) {
        exit(EXIT_FAILURE);
    }

    if(chattingClient(clientSocketDescriptor)) {
        exit(EXIT_FAILURE);
    }

    if(close(clientSocketDescriptor)) {
        perror(CLOSE_ERROR_MSG);
        exit(EXIT_FAILURE);
    }

    return 0;
}