#include <stdio.h>
#include <string>
#include <string.h>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

#ifndef HTTPSERVER_HTTPPARSER_H
#define HTTPSERVER_HTTPPARSER_H

#define CRLF "\r\n"
#define MAX_HEADERSP 30


class HttpParser {


public:

    typedef struct Header_{
        string key;
        string value;
    } Header;

    HttpParser(string msg);


    //GET
    string getMethod();

    bool isGet();

    bool isPost();

    string getHeader(string key);

    //zwraca zawsze z "/" na końcu
    string getUrl();


    //POST
    int postValuesSize();

    Header* postValues();



private:
    string whole_msg;
    string method_str;
    string Url;
    string http_ver;
    Header headers[MAX_HEADERSP];
    int headersNo;

    //only POST
    Header postvalues[MAX_HEADERSP];
    int postvals_no;

    void addHeader(string key, string value);
};




class ParserException: public exception{

    string cause;

public:

    ParserException(string cause){
        this->cause = cause;
    }

    virtual const char* what() const throw() {
        return this->cause.c_str();
    }
};


#endif //HTTPSERVER_HTTPPARSER_H
