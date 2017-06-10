#include "HttpParser.h"
using namespace std;


HttpParser::HttpParser(string msg){
    this->headersNo = 0;
    this->postvals_no = 0;
    this->whole_msg = msg;
    std::istringstream is( msg );
    std::getline( is, this->method_str, ' ' );
    std::getline( is, this->Url, ' ' );
    std::getline( is, this->http_ver, '\n' );


    if(this->method_str == "GET") {

        string tmp;

        while (std::getline(is, tmp, '\n')) {
            if (CRLF == "\r\n")
                tmp = tmp.substr(0, tmp.size() - 1);      //usuwamy pozostale \r

            string key = tmp.substr(0, tmp.find(":"));
            string value = tmp.substr(tmp.find(":") + 1, tmp.size() - 1);

            if (value[0] == ' ')
                value = value.substr(1, value.size() - 1);

            addHeader(key, value);
        }
    }

    else if(this->method_str == "POST"){
        string tmp;

        while (std::getline(is, tmp, '\n')) {
            if (CRLF == "\r\n")
                tmp = tmp.substr(0, tmp.size());      //usuwamy pozostale \r

            if(tmp[0] == '\n')
                break;


            string key = tmp.substr(0, tmp.find(":"));
            string value = tmp.substr(tmp.find(":") + 1, tmp.size() - 1);

            if (value[0] == ' ')
                value = value.substr(1, value.size() - 1);

            addHeader(key, value);
        }

        std::getline(is, tmp);


        while(1){                                            //example: key1=val1&key2=val2
            bool willBreak = false;
            string key, value;
            string token = tmp.substr(0, tmp.find("&"));    //token: key1=val1
            if(tmp.find("&") == string::npos)
                willBreak = true;

            key = token.substr(0, token.find("="));         //key: key1
            value = token.substr(token.find("=") + 1, token.size() - 1);    //value: val1
            tmp = tmp.substr(tmp.find("&") + 1, tmp.size() - 1);

            Header p;
            p.key = key;
            p.value = value;

            postvalues[postvals_no++] = p;

            if(willBreak)
                break;
        }
    }

    else{
        throw ParserException("Unknown method");
    }
}

string HttpParser::getMethod() {
    return this->method_str;
}

string HttpParser::getUrl() {
    return this->Url;
}

bool HttpParser::isGet() {
    return this->method_str == "GET";
}

bool HttpParser::isPost() {
    return this->method_str == "POST";
}

void HttpParser::addHeader(string key, string value){
    Header e;
    e.key = key;
    e.value = value;
    this->headers[headersNo++] = e;
}

string HttpParser::getHeader(string key){
    for(int i = 0; i < headersNo; i++){
        if(headers[i].key == key){
            return headers[i].value;
        }
    }
    return "";      //here exception
}

int HttpParser::postValuesSize() {
    return this->postvals_no;
}

HttpParser::Header *HttpParser::postValues() {
    return this->postvalues;
}
