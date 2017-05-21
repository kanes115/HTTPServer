//
// Created by Dominik Stanaszek on 19.05.2017.
//

#ifndef HTTPSERVERC_CLIENT_H
#define HTTPSERVERC_CLIENT_H


#include <cstdlib>
#include <string.h>
using namespace std;

class Client {

public:
    Client(std::string servAddress, std::string port);

    int waitForAcceptance();

    void closeSockets();


private:
    string destAddr, destPort;
    struct addrinfo *hints, *res;
    int sockfd;

    int prepareSocketAndConnect();
};


#endif //HTTPSERVERC_CLIENT_H
