#include <string>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

#ifndef HTTPSERVER_FILEMENAGER_H
#define HTTPSERVER_FILEMENAGER_H

using namespace std;


class FileMenager {

public:
    FileMenager(string root);

    string getPageFromFile(string relevantPath);


private:
    string root;
};


struct FileDoesNotExist : public exception {
    const char * what () const throw () {
        return "File not found";
    }
};


#endif //HTTPSERVER_FILEMENAGER_H
