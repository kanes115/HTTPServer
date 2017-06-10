#include "Configurator.h"
using namespace std;

Configurator::Configurator(string configfilePath){

    maxClients = -1;
    timeout = DEFAULT_TIMEOUT;
    port = -1;

    std::ifstream infile(configfilePath);

    std::string line;

    string tmp;
    int i = 0;

    while(std::getline(infile, tmp, '\n')){
        try {
            i++;
            if (tmp[0] == '#')   //komentarz
                continue;

            string key = tmp.substr(0, tmp.find(":"));
            string value = tmp.substr(tmp.find(":") + 1, tmp.size() - 1);

            if (value[0] == ' ')
                value = value.substr(1, value.size() - 1);

            classify(key, value);

        }catch(exception& e){
            throw ConfiguratorException("Badly formatted in line " + to_string(i) + "\n" +  string(e.what()));
        }
    }


}


Server* Configurator::config(){
    if(port == -1 || root == "" || maxClients == -1)
        throw ConfiguratorException("Not all of the mandatory fields specified.");

    return new Server(this->port, ".", this->root, this->maxClients, this->timeout, this->fileNotFoundPage,
                      this->logFile);
}



void Configurator::classify(string key, string value){
    if(key == "port"){
        try {
            this->port = stoi(value);
            if(to_string(this->port) != value)
                throw invalid_argument("");
        }catch(invalid_argument e){
            throw ConfiguratorException("Port must be an int, but is " + value);
        }
    }

    else if(key == "root"){
        if(value[0] == '/')
            throw ConfiguratorException("path should be relevant");
        this->root = value;
    }

    else if(key == "maxClients"){
        try {
            this->maxClients = stoi(value);
            if(to_string(this->maxClients) != value)
                throw invalid_argument("");
        }catch(invalid_argument e){
            throw ConfiguratorException("MaxClients must be an int, but is " + value);
        }
    }

    else if(key == "timeout"){
        try {
            this->timeout= stoi(value);
            if(to_string(this->timeout) != value)
                throw invalid_argument("");
        }catch(invalid_argument e){
            throw ConfiguratorException("Timeout must be an int, but is " + value);
        }
    }

    else if(key == "fileNotFoundPage"){
        if(value[0] == '/')
            throw ConfiguratorException("path should be relevant");
        this->fileNotFoundPage = value;
    }

    else if(key == "logFile"){
        if(value[0] == '/')
            throw ConfiguratorException("path should be relevant");
        this->logFile = value;
    }
    else{
        throw ConfiguratorException("Unknown key " + key);
    }
}