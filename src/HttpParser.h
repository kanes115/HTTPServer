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

    HttpParser(string msg);


    //GET
    string getMethod();

    bool isGet();

    bool isPost();

    string getHeader(string key);

    //zwraca zawsze z "/" na końcu
    string getUrl();


    //POST



private:
    typedef struct Header_{
        string key;
        string value;
    } Header;
    string whole_msg;
    string method_str;
    string Url;
    string http_ver;
    Header headers[MAX_HEADERSP];
    int headersNo;

    void addHeader(string key, string value);
};


#endif //HTTPSERVER_HTTPPARSER_H
