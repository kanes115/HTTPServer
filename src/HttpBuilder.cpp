//
// Created by Dominik Stanaszek on 21.05.2017.
//

#include "HttpBuilder.h"
using namespace std;

HttpBuilder::HttpBuilder() {
    this->headersNo = 0;
    statusCode = -1;
    reason = "";
    body = "";
}

//na pewno URL? jak to nazwać?
string HttpBuilder::buildGET(string URL){
    string res = "GET " + URL + " HTTP/" + PROTOCOL_VERSION;
    for(int i = 0; i < this->headersNo; i++){
        res += CRLF;
        res += this->headers[i];
    }
    res += CRLF;            //empty line indicating the end of headers
    return res;
}

void HttpBuilder::addHeader(string key, string value) {
    this->headers[headersNo] = key + ": " + value;
    this->headersNo++;
}

void HttpBuilder::setStatus(int status, string reason){
    this->statusCode = status;
    this->reason = reason;
}

void HttpBuilder::setBody(string body){
    this->body = body;
}

string HttpBuilder::buildResponse(){
    if(statusCode == -1 || reason == ""){           //na pewno mozna tak porównywać?
        cerr << "status code and reason must be set before buliding respone." << endl;
        return NULL;
    }
    //status line
    string res = "HTTP/";
    res += PROTOCOL_VERSION;
    res += " " + to_string(this->statusCode);
    res += " " + this->reason + CRLF;
    for(int i = 0; i < this->headersNo; i++){
        res += this->headers[i] + CRLF;
    }
    if(body != "")
        res += body + CRLF + CRLF + CRLF;
    return res;
}
