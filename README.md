# Chat Application using Socket API
## Objective
* To comprehend how the TCP specification is implemented in Berkeley Software Distribution (BSD) sockets (i.e., Berkeley sockets)
* To learn how to implement a network application using the socket API.
## Component
### Implementation part
* chat server
    * create a TCP socket
    * waiting for an incoming connection from the client
    * when the TCP connection is successfully connected, the client’s IP address and port number are printed
* chat client
    * establish a TCP connection to the server
### Description Part(Report)
* must address how the application works (both the server and the client), and the implementation details
* includes an execution environment, how to build and execute your programs, plus the screen dumps of the executions on the server and client.
## Requirements
### Environments
* MUST write the programs using C programming language on POSIX-compliant operating systems
* The BSD socket API is used to make the programs communicate with each other
### Chat Server
* **MUST** take one command-line argument as a port number for the program
* permits **only** one TCP client for the chatting program
* If the TCP connection is made, the client’s IP address and port number are printed to its standard output (e.g., "Connection from
163.152.162.144:12345")
* he received messages from the client should be printed on the screen of the server
* the server sends the string input from the user to the client
* Note that the server does not need to support multi-threading at this time
* The chatting program continues to run until the user enters the message "QUIT"
### Chat Client
* MUST take two command-line arguments: an IP address of the server, and a port number that the server designates for the application
* makes a TCP connection with the server using the command-line arguments in this application
* After establishing the successful connection, the message "Connected" is printed on the screen
* sends a message to the server first by user’s input
* receives a message from the server, and it should be printed to its standard output
* If the client sends (or receives) the message “QUIT”, the message “Disconnected” is printed on the screen, the socket is closed, and the program is terminated

## Referenced Sites
* https://badayak.com/entry/C언어-소켓에-IP주소와-포트번호-지정-함수-bind
* https://www.geeksforgeeks.org/socket-programming-cc/
* https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf
* https://www.tuwlab.com/ece/27193
* https://man7.org/linux/man-pages
* https://aticleworld.com/socket-programming-in-c-using-tcpip/
* https://gitlab.com/gilgil/sns/-/wikis/git-exam/git-exam
