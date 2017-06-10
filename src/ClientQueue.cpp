#include <cstdlib>
#include "ClientQueue.h"

using namespace std;


ClientQueue::ClientQueue(int size){
    this->size = size;
    tab = (Client**) malloc(size * sizeof(Client*));
    for(int i = 0; i < size; i++)
        tab[i] = NULL;
    if(size <= 0)
        throw std::invalid_argument("Size must be greater than 0");

    if(pthread_mutex_init(&clients_mutex, NULL) != 0){
        perror("pthread mutex init");
        exit(1);
    }
}


void ClientQueue::add(int id, thread *thr) {
    pthread_mutex_lock(&clients_mutex);
    Client* cl = (Client*) malloc(sizeof(Client));
    cl->id = id;
    cl->thr = thr;
    cl->toDelete = false;

    int i = 0;
    if(tab[i] == NULL) {
        tab[i] = cl;
        pthread_mutex_unlock(&clients_mutex);
        return;
    }

    i = (i + 1) % this->size;

    while(tab[i] != NULL){
        if(i == 0) {
            pthread_mutex_unlock(&clients_mutex);
            throw ClientQException("Queue is full");
        }
        i = (i + 1) % this->size;
    }

    tab[i] = cl;
    pthread_mutex_unlock(&clients_mutex);
}

void ClientQueue::remove(int id) {
    pthread_mutex_lock(&clients_mutex);
    for(int i = 0; i < size; i++){
        if(tab[i] != NULL && tab[i]->id == id) {
            free(tab[i]);
            tab[i] = NULL;
            pthread_mutex_unlock(&clients_mutex);
            return;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    throw ClientQException("No such element found");
}

Client* ClientQueue::get(int id){
    pthread_mutex_lock(&clients_mutex);
    for(int i = 0; i < size; i++){
        if(tab[i] != NULL && tab[i]->id == id) {
            pthread_mutex_unlock(&clients_mutex);
            return tab[i];
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    return NULL;
}

int ClientQueue::getSize() {
    return this->size;
}

bool ClientQueue::hasAtTabInd(int ind){
    return ind < size && tab[ind] != NULL;
}

Client* ClientQueue::getAtTabInd(int ind){
    pthread_mutex_lock(&clients_mutex);
    if(tab[ind] != NULL) {
        pthread_mutex_unlock(&clients_mutex);
        return tab[ind];
    }
    pthread_mutex_unlock(&clients_mutex);
    return NULL;
}

void ClientQueue::signToDelete(int id) {
    if(this->get(id) != NULL)
        this->get(id)->toDelete = true;

}
