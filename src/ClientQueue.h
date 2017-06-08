//
// Created by Dominik Stanaszek on 08.06.2017.
//

#ifndef HTTPSERVER_CLIENTQUEUE_H
#define HTTPSERVER_CLIENTQUEUE_H


/* Error Codes */

#define Q_FULL -1




typedef struct Client_{
    int id;
    thread *thr;
} Client;


class ClientQueue {

public:
    ClientQueue(int size);

    int add(int id, thread* thr);

    int remove(int id);

private:
    Client** tab;

};


#endif //HTTPSERVER_CLIENTQUEUE_H
