#include <string>
#include "schema.h"
#include "dataframe.h"
#include "column.h"
#include "SorParser.h"
#include "helper.h"
#include <vector>
#include "key.h"
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


vector<string> parseCommandLine(string s) {
    string temp = "";
    vector<string> vec;
    for (unsigned int i = 0; i < s.size(); i++) {
        if (s[i] == ' ') {
            if (temp != "") {
                vec.push_back(temp);
                temp = "";
            }
        } else {
            temp = temp + s[i];
        } 
    }
    if (temp != "") {
        vec.push_back(temp);
    }
    return vec;
}

string getType(Schema * s, int col) {
    char c = s->col_type(col);
    switch(c) {
        case 'B': {
            return "Boolean";
            break;
        }
        case 'I': {
            return "Integer";
            break;
        }
        case 'F': {
            return "Float";
            break;
        }
        case 'S': {
            return "String";
            break;
        }
        default: {
            return "Out of Bounds";
        }
    }
}

void getValue(DataFrame & df, Schema & s, int col, int row) {
    char c = s.col_type(col);
    switch(c) {
        case 'B': {
            std::cout << df.get_bool(col, row);
            break;
        }
        case 'I': {
            std::cout << df.get_int(col, row);
            break;
        }
        case 'F': {
            std::cout << df.get_float(col, row);
            break;
        }
        case 'S': {
            std::cout << df.get_string(col, row)->cstr_;
            break;
        }
        default: {
            std::cout << "Out of Bounds";
        }
    }
}

void printValues(DataFrame & df, Schema & s, unsigned int rowFrom, unsigned int colFrom, unsigned int rowTo, unsigned int colTo) {
    if (colTo < colFrom || rowTo < rowFrom || rowTo >= df.nrows() || colTo >= df.ncols()) {
        std::cout << "Range out of bounds" << std::endl;
        return;
    }
    for (unsigned int i = rowFrom; i <= rowTo; i++) {
        for (unsigned int j = colFrom; j <=colTo; j++) {
            getValue(df, s, j, i);
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}


// To be used like /sorer *Docname* "Number of Nodes" [IP ADDRESS] [PORT]
int main(int argh, char** argv) {
    int numNodes = atoi(argv[2]);
    if (argh != 3) {
        exit(1);
    }
    if (numNodes < 1) {
        exit(1);
    }
    SorParser * sor = new SorParser();
    sor->parseDocFile(argv[1], 0, 10000);
    char* val = sor->getSchema()->get()->c_str();
    Schema newSchema(val);
    DataFrame df(newSchema);
    Row r(newSchema);
    sor->setUpDF(&newSchema, &df);
    KVStore * kv = new KVStore();
    for (int i = 0; i < numNodes; i++) {
        int x = floor((i * df.nrows())/numNodes);
        int y = floor((i+1)*df.nrows()/numNodes);
        Key * k = new Key(to_string(i), i);
        kv->put(k, df.subset(x, y));
    }
    vector<DataFrame*> arr = kv->dfs;
    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr.at(i)->nrows() << std::endl;
    }

    std::cout << arr.at(1)->get_string(1,17)->c_str() << std::endl;

     //for the server, we only need to specify a port number
    if(argh != 5)
    {
        std::cerr << "Usage: port" << std::endl;
        exit(0);
    }
    //grab the port number
    int port = atoi(argv[4]);
    //buffer to send and receive messages with
    char msg[1500];
     
    //setup a socket and connection tools
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[3]);
    servAddr.sin_port = htons(port);
 
    //open stream oriented socket with internet address
    //also keep track of the socket descriptor
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        std::cerr << "Error establishing the server socket" << std::endl;
        exit(0);
    }
    //bind the socket to its local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        std::cerr << "Error binding socket to local address" << std::endl;
        exit(0);
    }
    std::cout << "Waiting for all clients to connect..." << std::endl;
    int* socketIDs = new int[atoi(argv[2])];
    int numClients = 0;
    while(numClients != atoi(argv[2])) {
        listen(serverSd, 5);
        sockaddr_in newSockAddr;
        socklen_t newSockAddrSize = sizeof(newSockAddr);
        int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
        if(newSd < 0)
        {
            std::cerr << "Error accepting request from client!" << std::endl;
            exit(1);
        } else {

            socketIDs[numClients] = newSd;
        
        }
        std::ofstream ofs("coltest");
        {
        boost::archive::text_oarchive oa(ofs);
        oa << kv.get(numClients);
        }
        std::ifstream ifs("coltest", std::ios::binary);
        std::string s = ifs.to_string();
        send(newSd, s.c_str(), s.size(), 0);
        recv(newSd, (char*)&msg, sizeof(msg), 0);
        numClients++;
    }
    
    std::cout << "Connected with clients!" << std::endl;
    //also keep track of the amount of data sent as well
    int turn = 0;
    while(1)
    {
        //receive a message from the client (listen)
        std::cout << "Awaiting client response..." << std::endl;
        memset(&msg, 0, sizeof(msg));//clear the buffer
        recv(socketIDs[turn], (char*)&msg, sizeof(msg), 0);
        std::cout << "Client" << turn << ": "<< msg << std::endl;
        char* toSend = new char[10];
        for (int i = 0; i < 1500; i++) {
            if (msg[i] == ' ') {
                break;
            } else {
                toSend[i] = msg[i];
                msg[i] = ' ';
            }
        }
        if (atoi(toSend) < numClients) {
            turn = atoi(toSend);
        }
        
        if(!strcmp(msg, "exit"))
        {
            std::cout << "Client has quit the session" << std::endl;
            break;
        }
        
        send(socketIDs[turn], (char*)&msg, strlen(msg), 0);
        
        memset(&msg, 0, sizeof(msg)); //clear the buffer
        
    }
    close(serverSd);
    std::cout << "********Session********" << std::endl;
    std::cout << "Connection closed..." << std::endl;
    return 0;   

    
}