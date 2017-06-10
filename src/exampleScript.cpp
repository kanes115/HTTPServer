#include <stdio.h>
#include <string>
#include <string.h>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;


//Example cgi script, it sends back login and crypted password

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Error" << endl;
        return -1;
    }
    string login = string(argv[1]);
    string password = string(argv[2]);

    cout<< "<!DOCTYPE html> <html lang=\"en\"> <head> </head>" << endl;

    cout << "<body>" << login << endl;

    for(int i = 0; i < password.size(); i++){
        cout << std::toupper(password[i]);
    }
    cout << endl;
    cout << "</body>" << endl;
    cout << "</html>" << endl;
}