#ifndef HTTPSERVER_HTTPBUILDER_H
#define HTTPSERVER_HTTPBUILDER_H

#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;


#define MAX_HEADERS 64
#define PROTOCOL_VERSION "1.1"
#define CRLF "\r\n"


/*
 * One instance of HttpBuilder should be used per each type of message
 * as it shares some fields between them (headers for example).
 */

class HttpBuilder {

public:
    HttpBuilder();


    //GET options
    string buildGET(string URL);

    void addHeader(string key, string value);

    //response options
    string buildResponse();

    void setStatus(int code, string reason);

    void setBody(string body);




private:
    //shared responses and GET
    string headers[MAX_HEADERS];
    int headersNo;
    //responses
    int statusCode;
    string reason;
    string body;

};


#endif //HTTPSERVER_HTTPBUILDER_H
