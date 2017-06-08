//
// Created by Dominik Stanaszek on 08.06.2017.
//

#include "ClientQueue.h"



ClientQueue::ClientQueue(int size){
    this->size = size;
    clients = (Client**) malloc(maxClients * sizeof(Client*));
    if(size <= 0)
        throw std::invalid_argument("Size must be greater than 0");
}


int ClientQueue::add(int id, thread *thr) {
    Client* cl = (Client*) malloc(sizeof(Client));
    cl->id = id;
    cl->thr = thr;

    int i = 0;
    if(tab[i] == NULL) {
        tab[i] = cl;
        return 0;
    }
    i = (i + 1) % this->size;

    while(tab[i] != NULL){
        if(i == 0)
            return Q_FULL;
    }

    clients[client_no] = cl;
}