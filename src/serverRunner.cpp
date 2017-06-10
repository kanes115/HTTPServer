#include <iostream>
#include "Server.h"
#include "Configurator.h"
using namespace std;

Server* s;


void sigintHandler(int signum){
    s->closeSockets();
    exit(0);
}


int main(int argc, char* argv[]) {
    signal(SIGINT, sigintHandler);
    signal(SIGALRM, sigintHandler);
    //alarm(15);
    if(argc != 2){
        std::cerr << "Usage: ./serverRunner <config file path>" << std::endl;
        exit(-1);
    }

    try {
        Configurator* c = new Configurator(argv[1]);
        s = c->config();

        string name = s->getHostname();
        cout << "Server started. Hostname: " << name << endl;
        s->runServer();

    }catch(exception& e){
        cout << e.what() << endl;
        exit(-1);
    }
    return 0;
}
