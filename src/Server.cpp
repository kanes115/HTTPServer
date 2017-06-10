#include "Server.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
// using std::string;

using namespace std;



//public
//rootFilePath - what should be run on "/" - passed excluding "/"
//fileNotFoundPagePath - what should be sent when file is not found - passed excluding "/"
Server::Server(int port, string root, string rootFilePath, int maxClients, int secTimeout = 15,
               string fileNotFoundPagePath = "", string logFilePath = "") {
    this->logger = new Logger(logFilePath);
    try{
        this->root = root;
        this->logFilePath = logFilePath;
        if(logFilePath == "")
            this->logger->deactivate();
        backlog = BACKLOG;
        this->maxClients = maxClients;
        this->toFinish = false;
        this->myport = to_string(port);
        this->fileMenager = new FileMenager(root);
        this->secTimeout = secTimeout;
        this->clients = new ClientQueue(maxClients);
        this->rootFile = rootFilePath;
        this->fileNotFoundPage = fileNotFoundPagePath;
        if(pthread_mutex_init(&connection_mutex, NULL) != 0){
            perror("pthread mutex init");
            exit(1);
        }
    } catch(exception e) {
        logger->log_error("Error while creating server\n" + string(e.what()));
        throw ServerException("Error while creating server\n" + string(e.what()));
    }
}


void Server::runServer(){
    try {
        prepareSocket();
        listenToPort();
        cout << "Port: " << this->myport << endl;
        logger->log("\nServer started: " + getHostname() + '\n' + this->listProperties());
        this->deleter = new thread(&Server::deletingTimeoutedClients, this);
        while(1) {
            struct sockaddr_storage* their_addr = (struct sockaddr_storage*) malloc(sizeof(struct sockaddr_storage));
            addr_size = sizeof(struct sockaddr_storage);
            int newfd = accept(listening_socket, (struct sockaddr *) their_addr, &addr_size);
            if(newfd == -1)
                throw ServerException("Accept error\n");
            logger->log("Got connection: " + getAddress(their_addr) + " id:  " + to_string(newfd));
            serve(newfd);
        }
    }catch(ServerException e){
        throw ServerException("Error while seeting up for listening on port\n" + string(e.what()));
    }
}

//NULL on error
string Server::getHostname() {
    char* myIP;
    myIP = (char *) malloc(64);

    if(gethostname(myIP, 64) != 0){
        throw ServerException("Error gethostname(): \n" + string(strerror(errno)));
    }
    return string(myIP);
}


void Server::closeSockets() {
    cout << "closing server..." << endl;
    logger->log("closing server...");
    toFinish = true;

    deleter->join();

    for(int i = 0; i < clients->getSize(); i++){
        pthread_mutex_lock(&connection_mutex);
        if(clients->hasAtTabInd(i)) {
            pthread_mutex_unlock(&connection_mutex);
            close(clients->getAtTabInd(i)->id);
            clients->getAtTabInd(i)->thr->join();
            pthread_mutex_lock(&connection_mutex);
        }
        pthread_mutex_unlock(&connection_mutex);
    }

    if(close(listening_socket) == -1)
        logger->log_error("Closing listening_socket: " + string(strerror(errno)));

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

void Server::prepareSocket(){
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;        // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;        // fill in my IP for me

    if(getaddrinfo(NULL, myport.c_str(), &hints, &res) != 0)
        throw ServerException("getaddrinfo() error: " + string(strerror(errno)));
    if((listening_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
        throw ServerException("socket() error" + string(strerror(errno)));

}


void Server::listenToPort(){
    if(::bind(listening_socket, res->ai_addr, res->ai_addrlen) != 0)
        throw ServerException("bind() error" + string(strerror(errno)));
    if(listen(listening_socket, backlog) != 0)
        throw ServerException("listen() error" + string(strerror(errno)));
}

void Server::serve(int new_fd){
    bool wasCaught = false;
    try{
        this->clients->add(new_fd, NULL);
    }catch(ClientQException& e){
        cout << "Client was refused" << endl;
        logger->log("Client was refused");
        close(new_fd);
        wasCaught = true;
    }
    if(!wasCaught)
        this->clients->get(new_fd)->thr = new thread(&Server::serve_routine, this, new_fd);
}



void Server::serve_routine(int myClientNo){
    struct timeval tv;
    tv.tv_sec = this->secTimeout;
    tv.tv_usec = 0;
    setsockopt(myClientNo, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval));
    while(1){
        char buf[MSGBUF_SIZE];
        ssize_t res = recv(myClientNo, buf, MSGBUF_SIZE, 0);

        if(res == 0){       //socket closed
            pthread_mutex_lock(&connection_mutex);
            clients->signToDelete(myClientNo);
            pthread_mutex_unlock(&connection_mutex);
            return;
        }

        if(res == -1 && (errno == EAGAIN || errno == EWOULDBLOCK || errno == ECONNRESET || errno == ENOTCONN)) {   //timeout or error
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                logger->log("Timeout for client " + to_string(myClientNo));
                cout << "Timeout for client " + to_string(myClientNo) << endl;
            }
            else {
                logger->log_error("Error when receiving from client " + to_string(myClientNo) + ": " + strerror(errno));
            }
            pthread_mutex_lock(&connection_mutex);
            clients->signToDelete(myClientNo);
            pthread_mutex_unlock(&connection_mutex);
            return;
        }

        if(toFinish) {
            pthread_mutex_lock(&connection_mutex);
            clients->signToDelete(myClientNo);
            pthread_mutex_unlock(&connection_mutex);
            return;
        }

        HttpParser *parser;
        try {
            parser = new HttpParser(string(buf));
            if (parser->isGet()) {
                cout << "Got GET query from client " << to_string(myClientNo) << endl;
                sendGetAnswer(parser, myClientNo);
            }
            else if (parser->isPost()) {
                cout << "Got POST query from client " << to_string(myClientNo) << endl;
                servePost(parser, myClientNo);
            }
        }catch(ServerException& e){
            logger->log_error("Error while sending message to client " + to_string(myClientNo));
            continue;
        }

        if(parser->getHeader("Connection") == "close") {
            pthread_mutex_lock(&connection_mutex);
            clients->signToDelete(myClientNo);
            pthread_mutex_unlock(&connection_mutex);
            delete parser;
            return;
        }
        delete parser;
    }
}





void Server::sendGetAnswer(HttpParser* parser, int myClientNo){
    logger->log("Got request from client " + to_string(myClientNo));
    HttpBuilder *resp = new HttpBuilder();
    string responseText;
    bool notFound = false;
    string body;
    try {
        string path;
        if(parser->getUrl() == "/")
            path = this->rootFile;
        else
            path = parser->getUrl().substr(1, parser->getUrl().size() - 1);
        body = this->fileMenager->getPageFromFile(path);
    } catch (FileDoesNotExist &e) {         //requested file not found
        resp->setStatus(404, e.what());
        if(this->fileNotFoundPage == "")
            resp->addHeader("Content-length", "0");
        else{
            try {
                body = this->fileMenager->getPageFromFile(this->fileNotFoundPage);
                resp->setBody(body);
                resp->addHeader("Content-length", resp->getBodySize());
            }catch(FileDoesNotExist& e){
                logger->log_error("\"File not found\" not found!");
            }
        }
        notFound = true;
        logger->log("Sending 404 file not found to client " + to_string(myClientNo));
    }
    if (!notFound) {
        resp->setBody(body);
        resp->addHeader("Content-length", resp->getBodySize());
        resp->setStatus(200, "OK");
        logger->log("Sending 200 to client " + to_string(myClientNo));
    }
    responseText = resp->buildResponse();
    if (sendMessage(myClientNo, responseText) != 0)
        throw ServerException("Error while sending message");

}



void Server::servePost(HttpParser *parser, int myClientNo) {
    string url = parser->getUrl();
    string scriptpath = url.substr(1, url.size() - 1);

    string cmd = this->root + "/";

    cmd += scriptpath;

    for(int i = 0; i < parser->postValuesSize(); i++)
        cmd += " " + parser->postValues()[i].value;

    FILE* descr = popen(cmd.c_str(), "r+");

    char buf[4096];
    string str_toclient = "";

    ssize_t readno;
    while((readno = fread(buf, 1, 4096, descr)) == 4096) {
        str_toclient += string(buf);
    }
    buf[readno] = '\0';
    str_toclient += string(buf);

    HttpBuilder *resp = new HttpBuilder();
    resp->setStatus(200, "OK");
    resp->setBody(str_toclient);
    resp->addHeader("Content-length", resp->getBodySize());

    cout << endl << "my response: " << resp->buildResponse() << endl;

    if (sendMessage(myClientNo, resp->buildResponse()) != 0)
        throw ServerException("Error while sending message");


}


int Server::sendMessage(int destSocket, string msg){
    if (send(destSocket, msg.c_str(), msg.length() + 1, 0) == 1){
        perror("send");
        return -1;
    }
    return 0;
}



void Server::deletingTimeoutedClients(){
    while(1) {
        sleep(DEL_CL_INTERVAL);
        pthread_mutex_lock(&connection_mutex);
        for (int i = 0; i < clients->getSize(); i++) {
            if (clients->hasAtTabInd(i) && clients->getAtTabInd(i)->toDelete) {
                cout << "Deleting client " << clients->getAtTabInd(i)->id << endl;
                logger->log("Deleting client " + to_string(clients->getAtTabInd(i)->id));
                if (shutdown(clients->getAtTabInd(i)->id, SHUT_RDWR) == -1) {
                    if(errno != ENOTCONN) {
                        printf("Couldn't shutdown connection with clients %d \n %s", clients->getAtTabInd(i)->id,
                               strerror(errno));
                        logger->log_error("Couldn't shutdown connection with client " +
                                          to_string(clients->getAtTabInd(i)->id) + ": \n\t" + strerror(errno));
                    }
                }
                if (close(clients->getAtTabInd(i)->id) == -1) {
                    printf("Couldn't close socket for clients %d \n %s", clients->getAtTabInd(i)->id, strerror(errno));
                    logger->log_error("Couldn't close socket for client " +
                                              to_string(clients->getAtTabInd(i)->id) + ": \n\t" + strerror(errno));
                }
                this->clients->remove(clients->getAtTabInd(i)->id);
            }
        }
        pthread_mutex_unlock(&connection_mutex);
        if(toFinish)
            return;
    }
}

string Server::listProperties() {
    string res;
    res = "\nProperties:\n";
    res = "\nport: " + this->myport + "\n";
    res += "at root: " + this->rootFile + "\n";
    res += "Max Clients: " + to_string(this->maxClients) + "\n";
    res += "Timeout: " + to_string(this->secTimeout) + "\n";
    if(this->fileNotFoundPage != "") res += "File Not Found Page Path: " + this->fileNotFoundPage + "\n";
    if(this->logFilePath != "") res += "Log filepath: " + this->logFilePath + "\n";
    return res;
}


#pragma clang diagnostic pop
