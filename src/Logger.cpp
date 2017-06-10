#include "Logger.h"

using namespace std;


Logger::Logger(string logFilePath) {
    this->logPath = logFilePath;
    if(pthread_mutex_init(&file_lock, NULL) != 0){
        perror("pthread mutex init");
        throw LoggerException("Could not init a mutex");
    }
    this->activated = true;
}


void Logger::deactivate(){
    pthread_mutex_lock(&file_lock);
    this->activated = false;
    pthread_mutex_unlock(&file_lock);
}

void Logger::activate(){
    pthread_mutex_lock(&file_lock);
    this->activated = true;
    pthread_mutex_unlock(&file_lock);
}

void Logger::log(string msg) {
    if(!activated)
        return;
    pthread_mutex_lock(&file_lock);

    string res_log = "\n" + getTimestamp() + msg;

    std::ofstream out;

    out.open(this->logPath, std::ios::app);

    std::string str = res_log;
    out << str;
    pthread_mutex_unlock(&file_lock);
}


void Logger::log_error(string msg) {
    if(!activated)
        return;
    pthread_mutex_lock(&file_lock);

    string res_log = "\n*** ERROR *** " + getTimestamp() + msg;

    std::ofstream out;

    out.open(this->logPath, std::ios::app);

    std::string str = res_log;
    out << str;
    pthread_mutex_unlock(&file_lock);
}


string Logger::getTimestamp() {
    time_t t = time(0);
    struct tm* now = localtime(&t);
    string res_log = "[";
    res_log += to_string(now->tm_year + 1900);
    res_log += DATE_DELIM;
    res_log += to_string(now->tm_mon + 1);
    res_log += DATE_DELIM;
    res_log += to_string(now->tm_mday);

    res_log += "  ";
    res_log += to_string(now->tm_hour);
    res_log += TIME_DELIM;
    res_log += to_string(now->tm_min);
    res_log += TIME_DELIM;
    res_log += to_string(now->tm_sec);

    res_log += "]";

    return res_log;
}