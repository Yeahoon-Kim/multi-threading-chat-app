#include "chatProgram.h"

/*
* Print error and close socket file descriptor
* socketfd : Socket file descriptor number which has to be closed
* msg      : Error message printed
*/
void handleError(int sockfd, const char * msg) {
    fputs(msg, stderr);                 // Print error message
    sendMessage(sockfd, "QUIT\n", 5);   // Send QUIT message
    close(sockfd);                      // Close socket file descriptor
}

/*
* Receive message from network socket
* sockfd : Socket file descriptor number from which message is received
* buffer : Data storage
* buflen : Buffer length
*/
int recvMessage(int sockfd, char* buffer, int bufLen) {
    int inputLength = recv(sockfd, buffer, bufLen - 1, 0);  // Receive message from socket
    if(inputLength <= 0) return RECV_FAILURE;               // Return error if message length is zero

    buffer[inputLength] = '\0';                             // Remove line feed character

    if(!strncmp(buffer, "QUIT", 4)) return RECV_END;        // Quit connection
    else return RECV_SUCCESS;
}

/*
* Send message to network socket
* sockfd : Socket file descriptor number to which message is sent
* buffer : Data storage
* buflen : Buffer length
*/
int sendMessage(int sockfd, char* buffer, int bufLen) {
    send(sockfd, buffer, bufLen, 0);                        // Send message to socket
    if(!strncmp(buffer, "QUIT", 4)) return SEND_END;        // Quit connection
    else return SEND_SUCCESS;
}

/*
* Get message from user and send it to network socket
* sockfd : Socket file descriptor number to which message is sent
* buffer : Data storage
* buflen : Buffer length
*/
int scanAndSendMessage(int sockfd, char* buffer, int bufLen) {
    int inputLength = read(0, buffer, bufLen - 1);          // Receive message from user
    if(inputLength <= 0) return SEND_FAILURE;               // Return error if message length is zero

    buffer[inputLength] = '\0';                             // Remove line feed character

    send(sockfd, buffer, inputLength, 0);                   // Send message to socket
    if(!strncmp(buffer, "QUIT", 4)) return SEND_END;        // Quit connection
    else return SEND_SUCCESS;
}