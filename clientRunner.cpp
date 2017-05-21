//
// Created by Dominik Stanaszek on 20.05.2017.
//

#include <iostream>
#include "Client.h"

Client* c;


void sigintHandler(int signum){
    c->closeSockets();
    exit(0);
}


int main(int argc, char* argv[]) {
    if(argc != 3){
        std::cout << "Wrong usage" << std::endl;
        exit(-1);
    }
    signal(SIGINT, sigintHandler);

    c = new Client(argv[1], argv[2]);

    c->waitForAcceptance();

    return 0;
}