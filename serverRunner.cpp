#include <iostream>
#include "Server.h"
using namespace std;

Server* s;


void sigintHandler(int signum){
    s->closeSockets();
    exit(0);
}


int main(int argc, char* argv[]) {
    signal(SIGINT, sigintHandler);
    if(argc != 2){
        printf("Wrong usage\n");
        exit(-1);
    }

    s = new Server(5, argv[1]);

    char* name = s->getHostname();
    printf("Server started. Hostname: %s\n", name);
    free(name);
    s->runServer();




    return 0;
}