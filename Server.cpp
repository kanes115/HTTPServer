#include <string>
#include "Server.h"
using std::string;



//public

Server::Server(int maxConnections, string port){
    backlog = maxConnections;
    this->myport = port;
}


void Server::runServer(){

    if(prepareSocket() != 0 || listenToPort() != 0){
        printf("Error while preparing sockets and listening to port\n");
        exit(-1);
    }

    while(1) {
        addr_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);
        printf("Client came\n");
        if(!fork()){
            close(sockfd);
            if (send(new_fd, "I can hear you!\n", sizeof("I can hear you!\n"), 0) == 1){
                perror("send");
                exit(-1);
            }
            close(new_fd);
            exit(0);
        }
    }
}

//must be freed
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
