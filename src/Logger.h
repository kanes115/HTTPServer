#ifndef HTTPSERVER_LOGGER_H
#define HTTPSERVER_LOGGER_H

#include <thread>
#include <pthread.h>
#include <iostream>
#include <ctime>
#include <time.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>


#define DATE_DELIM "."
#define TIME_DELIM ":"

using namespace std;


class Logger {

public:

    Logger(string logFilePath);

    void log(string msg);


    void activate();

    void deactivate();

    void log_error(string msg);

private:

    pthread_mutex_t file_lock;

    string logPath;

    bool activated;

    string getTimestamp();
};


class LoggerException: public exception{

    string cause;

public:

    LoggerException(string cause){
        this->cause = cause;
    }

    virtual const char* what() const throw() {
        return this->cause.c_str();
    }
};


#endif //HTTPSERVER_LOGGER_H
