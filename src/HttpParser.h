#include <stdio.h>
#include <string>
#include <string.h>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

#ifndef HTTPSERVER_HTTPPARSER_H
#define HTTPSERVER_HTTPPARSER_H


class HttpParser {


public:

    HttpParser(string msg);


    //GET
    string getMethod();

    //zwraca zawsze z "/" na końcu
    string getUrl();


    //POST



private:
    string method;
    string Url;

};


#endif //HTTPSERVER_HTTPPARSER_H
