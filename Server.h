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


#include <netinet/in.h>

using namespace std;




class Server {

public:
    Server(int maxConnections, std::string port);

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

    int prepareSocket();

    int listenToPort();
};



#endif //HTTPSERVERC_SERVER_H
