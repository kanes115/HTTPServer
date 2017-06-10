#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <thread>
#include <pthread.h>
#include <string.h>
#include <ctime>
#include <time.h>
#include "FileMenager.h"
#include "HttpBuilder.h"
#include "HttpParser.h"
#include "ClientQueue.h"
#include "Logger.h"

#ifndef HTTPSERVERC_SERVER_H
#define HTTPSERVERC_SERVER_H

#define MSGBUF_SIZE 1024 * 64
#define DEL_CL_INTERVAL 1

#define BACKLOG 5


class Server {

public:
    Server(int port, string root, string rootFilePath, int maxClients, int secTimeout,
               string fileNotFoundPagePath, string logFilePath);

    void runServer();

    string getHostname();

    void closeSockets();


private:

    int listening_socket;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int backlog;
    string myport;
    ClientQueue *clients;
    //mutexes
    pthread_mutex_t connection_mutex;
    bool toFinish;
    thread* deleter;
    Logger* logger;




  void serve_routine(int myClientNo);

  string getAddress(struct sockaddr_storage* addrb);

  void prepareSocket();

  void listenToPort();

  FileMenager* fileMenager;

  void serve(int new_fd);

  int sendMessage(int destSocket, string msg);

  int secTimeout;

    void sendGetAnswer(HttpParser *parser, int socket);

    void servePost(HttpParser *parser);

    void deletingTimeoutedClients();

    string rootFile;
    string fileNotFoundPage;

    string listProperties();

    int maxClients;
    string logFilePath;
};


class ServerException: public exception{

    string cause;

public:

    ServerException(string cause){
        this->cause = cause;
    }

    virtual const char* what() const throw() {
        return this->cause.c_str();
    }
};



#endif //HTTPSERVERC_SERVER_H
