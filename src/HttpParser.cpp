#include "HttpParser.h"
using namespace std;


HttpParser::HttpParser(string msg){
    std::istringstream is( msg );
    std::getline( is, this->method, ' ' );
    std::getline( is, this->Url, ' ' );
}

string HttpParser::getMethod() {
    return this->method;
}

string HttpParser::getUrl() {
    return this->Url;
}