#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <pthread.h>
#include "FileMenager.h"
#include "HttpBuilder.h"
#include "HttpParser.h"

#ifndef HTTPSERVERC_SERVER_H
#define HTTPSERVERC_SERVER_H

using namespace std;


#define TIMEOUT 15


class Server {

public:
    Server(int maxConnections, string port, string root);

    void runServer();

    string getHostname();

    void closeSockets();


private:
    int sockfd, new_fd;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int backlog;
    string myport;

    HttpBuilder* responesBuilder;

    int prepareSocket();

    int listenToPort();

    FileMenager* fileMenager;

    void serve(int new_fd);

    int sendMessage(int destSocket, string msg);
};


#endif //HTTPSERVERC_SERVER_H
