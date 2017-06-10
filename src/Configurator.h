#import "Server.h"

#ifndef HTTPSERVER_CONFIGURATOR_H
#define HTTPSERVER_CONFIGURATOR_H

using namespace std;

#define DEFAULT_TIMEOUT 15

class Configurator {

public:
    Configurator(string configfilePath);

    Server* config();

private:

    int port;
    string root;
    int maxClients;
    int timeout;
    string fileNotFoundPage;
    string logFile;

    void classify(string key, string value);


};







class ConfiguratorException: public exception{

    string cause;

public:

    ConfiguratorException(string cause){
        this->cause = cause;
    }

    virtual const char* what() const throw() {
        return this->cause.c_str();
    }
};


#endif //HTTPSERVER_CONFIGURATOR_H
