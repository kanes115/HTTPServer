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
        std::cerr << "Wrong usage" << std::endl;
        exit(-1);
    }

    s = new Server(5, argv[1]);

    string name = s->getHostname();
    cout << "Server started. Hostname: " << name << endl;
    s->runServer();




    return 0;
}