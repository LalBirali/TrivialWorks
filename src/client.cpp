#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

// Credit to Karim Oumghar
// https://simpledevcode.wordpress.com/2016/06/16/client-server-chat-in-c-using-sockets/
// We used his server.cpp and client.cpp and made modifications to support multiple clients

//Client side - to be used like ./client -ip [Client-IP] [Client-Port]
int main(int argc, char *argv[])
{
    //we need 2 things: ip address and port number, in that order
    if(argc != 4)
    {
        std::cerr << "Usage: ip_address port" << std::endl; exit(0); 
    } //grab the IP address and port number 
    char *serverIp = "127.0.0.1"; int port = 10000; 
    //create a message buffer 
    char msg[1500]; 
    //setup a socket and connection tools 
    struct hostent* host = gethostbyname(serverIp); 
    sockaddr_in clientAddr;
    bzero((char*)&clientAddr, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = inet_addr(argv[2]);
    clientAddr.sin_port = htons(atoi(argv[3]));
    sockaddr_in sendSockAddr;   
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = 
        inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    int bindStatus = bind(clientSd, (struct sockaddr*) &clientAddr, 
        sizeof(clientAddr));
    if(bindStatus < 0)
    {
        std::cerr << "Error binding socket to local address" << std::endl;
        exit(0);
    }
    //try to connect...
    int status = connect(clientSd,
                         (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        std::cout<<"Error connecting to socket!"<<std::endl;
        exit(1);
    }
    std::cout << "Connected to the server!" << std::endl;
    recv(clientSd, (char*)&msg, sizeof(msg), 0);
    std::cout << msg << std::cout;
    char * space = " ";
    strcpy(msg, argv[2]);
    strcpy(msg, space);
    strcpy(msg, argv[3]);
    send(clientSd, (char*)&msg, strlen(msg), 0);
    bool myTurn = false;
    bool sendIP = true;
    while(1)
    {
        
        if (myTurn) {
            std::cout << ">";
            std::string data;
            getline(std::cin, data);
            memset(&msg, 0, sizeof(msg));//clear the buffer
            strcpy(msg, data.c_str());
            if(data == "exit")
            {
                send(clientSd, (char*)&msg, strlen(msg), 0);
                break;
            }
            send(clientSd, (char*)&msg, strlen(msg), 0);
            myTurn = false;
        }
        
        memset(&msg, 0, sizeof(msg));//clear the buffer
        recv(clientSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit"))
        {
            std::cout << "Server has quit the session" << std::endl;
            break;
        }
        std::cout << msg << std::endl;
        myTurn = true;
    }
    close(clientSd);
    std::cout << "Connection closed" << std::endl;
    return 0;    
}