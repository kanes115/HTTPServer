#include "Server.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
// using std::string;



//public

Server::Server(int maxConnections, string port, string root, int secTimeout, int maxClients) {
    backlog = maxConnections;
    this->myport = port;
    this->responesBuilder = new HttpBuilder();
    this->fileMenager = new FileMenager(root);
    this->secTimeout = secTimeout;
    clients = (Client**) malloc(maxClients * sizeof(Client*));
    client_no = 0;
    if(pthread_mutex_init(&clients_mutex, NULL) != 0){
      perror("pthread mutex init");
      exit(1);
    }
}


void Server::runServer(){

    if(prepareSocket() != 0 || listenToPort() != 0){
        cerr << "Error while preparing sockets and listening to port" << endl;
        exit(-1);
    }

    while(1) {
        struct sockaddr_storage* their_addr = (struct sockaddr_storage*) malloc(sizeof(struct sockaddr_storage));
        addr_size = sizeof(struct sockaddr_storage);
        int newfd = accept(listening_socket, (struct sockaddr *) their_addr, &addr_size);
        cout << "Got connection! " << getAddress(their_addr) << endl;
        serve(newfd);
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
  if(shutdown(listening_socket, SHUT_RDWR) == -1)
    printf("Couldn't shutdown listening socket\n");

  for(int i = 0; i < client_no; i++){
      if(shutdown(clients[i]->socket, SHUT_RDWR) == -1){
        printf("Couldn't shutdown connection with clients %d \n", i);
      }
      if(close(clients[i]->socket) == -1){
        printf("Couldn't close socket for clients %d \n", i);
      }
  }
  if(close(listening_socket) == -1){
    printf("Couldn't close socket for listening_socket\n");
  }
}






//private

string Server::getAddress(struct sockaddr_storage* addrb){
  sa_family_t my_family = addrb->ss_family;
  if(my_family == AF_INET){
    struct sockaddr_in* resAddr = (sockaddr_in*) addrb;
    char dst[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET, resAddr, dst, INET6_ADDRSTRLEN);
    return string(dst);
  }else{
    struct sockaddr_in6* resAddr = (sockaddr_in6*) addrb;
    char dst[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, resAddr, dst, INET6_ADDRSTRLEN);
    return string(dst);
  }
}

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
    if((listening_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){
        perror("socket");
        return -2;
    }
    printf("listening_socket in server is %d\n", listening_socket);
    return 0;
}


int Server::listenToPort(){

    if(::bind(listening_socket, res->ai_addr, res->ai_addrlen) != 0){
      perror("bind");
      exit(1);
    }
    if(listen(listening_socket, backlog) != 0){
        perror("listen");
        return -2;
    }
    return 0;
}

//need to implement timeout
void Server::serve(int new_fd){
  pthread_mutex_lock(&clients_mutex);
  Client* cl = (Client*) malloc(sizeof(Client));
  cl->socket = new_fd;
  clients[client_no] = cl;
  thread(&Server::serve_routine, this);
  // if(pthread_create(&cl->t_id, NULL, serve_routine, this) != 0){
  //   perror("pthread_create");
  //   return;
  // }
  pthread_mutex_lock(&incr_mutex);
  client_no++;

  pthread_mutex_unlock(&clients_mutex);
}



void Server::serve_routine(){
  pthread_mutex_lock(&this->incr_mutex);
  pthread_mutex_unlock(&this->incr_mutex);
    while(1){
        char buf[255];
        string responseText;
        if (recv(this->clients[client_no]->socket, buf, 255, 0)) {
            HttpBuilder *resp = new HttpBuilder();
            HttpParser *parser = new HttpParser(string(buf));
            bool notFound = false;
            string body;
            try {
                body = this->fileMenager->getPageFromFile(parser->getUrl());
            } catch (FileDoesNotExist &e) {
                resp->setStatus(404, "File_not_found");
                notFound = true;
            }

            if (!notFound) {
                resp->setBody(body);
                resp->setStatus(200, "OK");
            }

            responseText = resp->buildResponse();

            if (sendMessage(this->clients[client_no]->socket, responseText) != 0) {
                cerr << "Error while sending message" << endl;
                exit(-1);
            }
        }
    }
    close(this->clients[client_no]->socket);
}



int Server::sendMessage(int destSocket, string msg){
    if (send(destSocket, msg.c_str(), msg.length() + 1, 0) == 1){
        perror("send");
        return -1;
    }
    return 0;
}

#pragma clang diagnostic pop
