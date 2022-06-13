# Chat Application using Socket API and Multi-Threading
## Objective
* To comprehend how the TCP specification is implemented in Berkeley Software Distribution (BSD) sockets (i.e., Berkeley sockets)
* To learn how to implement a network application using the socket API.
## Component
### Implementation part
* chat server
    * create a TCP socket
    * waiting for an incoming connection from the client
    * when the TCP connection is successfully connected, the client’s IP address and port number are printed
    * make new thread for managing connection with each clients
* chat client
    * establish a TCP connection to the server
    * make new thread for receiving message from server
### Description Part(Report)
* must address how the application works (both the server and the client), and the implementation details
* includes an execution environment, how to build and execute your programs, plus the screen dumps of the executions on the server and client.
## Requirements
### Environments
* MUST write the programs using C programming language on POSIX-compliant operating systems
* The BSD socket API is used to make the programs communicate with each other
### Chat Server
* MUST take one command-line argument as a port number for the program
* The server **only** accepts up to 5 TCP clients for the chatting program 
* If the TCP connection is made with a client, the client’s IP address and port number are printed to its **standard output** (e.g., “Connection from 163.152.162.144:12345”)
* After the connection, the server receives the nickname of the connected client (e.g., "User1", "User2")
* Whenever a new client connects to the server, the server uses client's nickname to notify **other connected clients** of the new connection and also displays it on the **server's screen** (e.g., "User2 is connected")
* If the server receives a message from a client, it should be printed on **the screens of the server** and
  **other clients**
  * On the screens of the server and other clients, **the nickname of the client** who sent the message should be printed together in front of the message (e.g., "User1: Hello") 
* On the server side, it does **not** receive a string from the prompt but **only sends the received message from each client** to other clients with the nickname of a client
* The received message is **not** sent back to the client that sent the message 
* The server communicates with multiple clients using **multithreading**
  * The process of receiving and delivering messages over several sockets by the server should be done **simultaneously**, not sequentially 
* If there is a variable shared by multiple threads, you should set a **critical section** so that only one thread can access the variable at a time, and modify the variable only within the critical section
  * Otherwise, the result may be different depending on the order of threads accessing the variable
* If a client named User2 sends the message "QUIT” to the server, the server sends the message "User2 is disconnected” to **other clients** and also prints it on server's screen
  * the server closes its socket communicating with the client named User2
### Chat Client
* MUST take three command-line arguments: an IP address of the server, a port number that the server designates for the application, and the nickname of the client. 
* Makes a TCP connection with the server using the command-line arguments (an IP address and a port number) in this application 
* After establishing the successful connection, the client sends its nickname to the server first by using the third command-line argument
* A message meaning the successful connection state and its nickname is displayed on the screens of the **server** and **all the clients including itself** (e.g., "User1 is connected”)
* Message should be printed with the nickname of the sending client (e.g., "User2: Hello")
  * However, if a client (i.e., User2) sends a message, it can output the message on its own screen without its nickname (e.g., "Hello")

* The client need to use **multi-threading** for its simultaneous transmission and reception
* If a client named User2 sends the message "QUIT", the message "User2 is disconnected” is printed
  on the screens of a server and all the clients including User2 
  * The client entering "QUIT” closes its socket and the client program is terminated.


## Referenced Sites
* https://badayak.com/entry/C언어-소켓에-IP주소와-포트번호-지정-함수-bind
* https://www.geeksforgeeks.org/socket-programming-cc/
* https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf
* https://www.tuwlab.com/ece/27193
* https://man7.org/linux/man-pages
* https://aticleworld.com/socket-programming-in-c-using-tcpip/
* https://gitlab.com/gilgil/sns/-/wikis/git-exam/git-exam
