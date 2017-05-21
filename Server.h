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

#ifndef HTTPSERVERC_SERVER_H
#define HTTPSERVERC_SERVER_H

#define MYPORT "3049"


#include <netinet/in.h>




class Server {

public:
    Server(int maxConnections, char* port);

    void runServer();

    char* getHostname();

    void closeSockets();


private:
    int sockfd, new_fd;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int backlog;
    char *myport;
    char myIP[INET6_ADDRSTRLEN];

    int prepareSocket();

    int listenToPort();
};



#endif //HTTPSERVERC_SERVER_H
