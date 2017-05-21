//
// Created by Dominik Stanaszek on 19.05.2017.
//

#include <netdb.h>
#include <cstring>
#include <cstdio>
#include <zconf.h>
#include <signal.h>
#include "Client.h"


//public

Client::Client(char *servAddress, char *port) {
    this->destAddr = (char *) malloc(strlen(servAddress));
    this->destPort = (char *) malloc(strlen(port));
    strcpy(destAddr, servAddress);
    strcpy(destPort, port);
}

int Client::waitForAcceptance(){

    if(prepareSocketAndConnect() != 0){
        printf("Error while preparing sockets and connecting\n");
        return -1;
    }

    char buf[255];

    while(1){
        usleep(10);
        if(recv(sockfd, buf, 255, 0)){
            printf("I got message: %s\n", buf);
            return 0;
        }
    }
}

void Client::closeSockets() {
    close(sockfd);
}



//private

int Client::prepareSocketAndConnect() {
    hints = (addrinfo *) malloc(sizeof(struct addrinfo));
    memset(hints, 0, sizeof(addrinfo));
    hints->ai_family = AF_UNSPEC;
    hints->ai_socktype = SOCK_STREAM;

    if(getaddrinfo(destAddr, destPort, hints, &res) != 0){
        perror("getaddrinfo");
        return -1;
    }
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sockfd == -1){
        perror("socket");
        return -2;
    }

    if(connect(sockfd, res->ai_addr, res->ai_addrlen) == -1){
        perror("connect");
        return -3;
    }
    return 0;
}
