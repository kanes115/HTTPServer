#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <thread>
#include <pthread.h>
#include "FileMenager.h"
#include "HttpBuilder.h"
#include "HttpParser.h"

#ifndef HTTPSERVERC_SERVER_H
#define HTTPSERVERC_SERVER_H

// using namespace std;


#define TIMEOUT 15


class Server {

public:
    Server(int maxConnections, string port, string root, int secTimeout, int maxClients);

    void runServer();

    string getHostname();

    void closeSockets();


private:
  typedef struct Client_{
    int socket;
      thread *thr;
  } Client;

  int listening_socket;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int backlog;
  string myport;

  //table of clients
  Client** clients;
  int client_no;
  pthread_mutex_t clients_mutex;

  pthread_mutex_t incr_mutex;

  HttpBuilder* responesBuilder;



  void serve_routine(int myClientNo);

  string getAddress(struct sockaddr_storage* addrb);

  int prepareSocket();

  int listenToPort();

  FileMenager* fileMenager;

  void serve(int new_fd);

  int sendMessage(int destSocket, string msg);

  int secTimeout;
};


#endif //HTTPSERVERC_SERVER_H
