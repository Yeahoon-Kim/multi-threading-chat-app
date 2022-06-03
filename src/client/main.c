#include "chatClient.h"

int clientSocketDescriptor;
volatile int isRecvThreadTerminate;

/*
* Socket 통신을 위한 Client 프로그램 작성
* Server : socket() -> bind() -> listen() -> accept() -> send() -> recv()
* Client : socket() ->                connect() ->       recv() -> send() -> close()
*/
int main(int argc, char* argv[]) {
    signal(SIGINT, clientInterruptHandler);

    if(argc != 4) {
        printUsageError();
        exit(EXIT_FAILURE);
    }

    // Limit user nickname to 20
    if(strlen(argv[3]) > 20) {
        printUserNameLengthError();
        exit(EXIT_FAILURE);
    }

    if(clientSocketSetting(argv[1], argv[2]) == SETTING_FAILURE) {
        exit(EXIT_FAILURE);
    }

    if(chattingClient(argv[3]) != CHATTING_SUCCESS) exit(EXIT_FAILURE);
    
    if(close(clientSocketDescriptor)) {
        fputs(CLOSE_ERROR_MSG, stderr);
        exit(EXIT_FAILURE);
    }

    return 0;
}