#include "Server.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
using std::string;



//public

Server::Server(int maxConnections, string port, string root) {
    backlog = maxConnections;
    this->myport = port;
    this->responesBuilder = new HttpBuilder();
    this->fileMenager = new FileMenager(root);
}


void Server::runServer(){

    if(prepareSocket() != 0 || listenToPort() != 0){
        cerr << "Error while preparing sockets and listening to port" << endl;
        exit(-1);
    }

    while(1) {
        addr_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);
        cout << "Got connection!" << endl;
        serve(new_fd);
    }
}

//NULL on error
string Server::getHostname() {
    char* myIP;
    myIP = (char *) malloc(64);

    if(gethostname(myIP, 64) != 0){
        perror("gethostname");
        return NULL;
    }

    return string(myIP);
}


void Server::closeSockets() {
    close(sockfd);
    close(new_fd);
}






//private


int Server::prepareSocket(){
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me


    if(getaddrinfo(NULL, myport.c_str(), &hints, &res) != 0){
        perror("getaddrinfo");
        return -1;
    }

    // make a socket, bind it, and listen on it:

    if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){
        perror("socket");
        return -2;
    }

    printf("sockfd in server is %d\n", sockfd);

    return 0;
}


int Server::listenToPort(){
    if(bind(sockfd, res->ai_addr, res->ai_addrlen) != 0){
        perror("bind");
        return -1;
    }
    if(listen(sockfd, backlog) != 0){
        perror("listen");
        return -2;
    }
    return 0;
}

//need to implement timeout
void Server::serve(int new_fd){

    if(!fork()){

        while(1) {
            char buf[255];
            string responseText;
            if (recv(new_fd, buf, 255, 0)) {
                HttpBuilder *resp = new HttpBuilder();
                HttpParser *parser = new HttpParser(string(buf));
                bool notFound = false;
                string body;
                try {
                    body = fileMenager->getPageFromFile(parser->getUrl());
                } catch (FileDoesNotExist &e) {
                    resp->setStatus(404, "File_not_found");
                    notFound = true;
                }

                if (!notFound) {
                    resp->setBody(body);
                    resp->setStatus(200, "OK");
                }

                responseText = resp->buildResponse();

                cout << "My response: " << responseText << endl;

                if (sendMessage(new_fd, responseText) != 0) {
                    cerr << "Error while sending message" << endl;
                    exit(-1);
                }
            }
        }
        close(new_fd);
    }
}



int Server::sendMessage(int destSocket, string msg){
    if (send(destSocket, msg.c_str(), msg.length() + 1, 0) == 1){
        perror("send");
        return -1;
    }
    return 0;
}

#pragma clang diagnostic pop