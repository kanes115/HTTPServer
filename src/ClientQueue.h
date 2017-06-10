//
// Created by Dominik Stanaszek on 08.06.2017.
//
#include <thread>
#include <pthread.h>
#include <iostream>

#ifndef HTTPSERVER_CLIENTQUEUE_H
#define HTTPSERVER_CLIENTQUEUE_H

using namespace std;


/* Error Codes */

#define Q_FULL -1
#define Q_NOELEM -2



typedef struct Client_{
    int id;
    thread *thr;
    bool toDelete;
} Client;


class ClientQueue {

public:
    ClientQueue(int size);

    void add(int id, thread* thr);

    void remove(int id);

    Client* get(int id);

    int getSize();

    bool hasAtTabInd(int id);

    Client *getAtTabInd(int ind);

    void signToDelete(int id);

private:
    Client** tab;

    pthread_mutex_t clients_mutex;

    int size;
};



class ClientQException: public exception{

    const char* cause;

public:
    ClientQException(const char *cause){
        this->cause = cause;
    }

    virtual const char* what() const throw() {
        return this->cause;
    }
};


#endif //HTTPSERVER_CLIENTQUEUE_H
