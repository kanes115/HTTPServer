#ifndef HTTPSERVERC_CLIENT_H
#define HTTPSERVERC_CLIENT_H


#include <cstdlib>
#include <string.h>

#include "HttpBuilder.h"
using namespace std;





class Client {

public:
    Client(string servAddress, string port);

    int waitForAcceptance();

    void closeSockets();


private:
    HttpBuilder *msgBuilder;
    string destAddr, destPort;
    struct addrinfo *hints, *res;
    int sockfd;

    int prepareSocketAndConnect();
};


#endif //HTTPSERVERC_CLIENT_H
