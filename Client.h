//
// Created by Dominik Stanaszek on 19.05.2017.
//

#ifndef HTTPSERVERC_CLIENT_H
#define HTTPSERVERC_CLIENT_H


#include <cstdlib>

class Client {

public:
    Client(char *servAddress, char *port);

    int waitForAcceptance();

    void closeSockets();


private:
    char *destAddr, *destPort;
    struct addrinfo *hints, *res;
    int sockfd;

    int prepareSocketAndConnect();
};


#endif //HTTPSERVERC_CLIENT_H
