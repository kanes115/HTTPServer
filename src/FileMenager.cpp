#include "FileMenager.h"

using namespace std;

FileMenager::FileMenager(string root){
    this->root = root;
}

string FileMenager::getPageFromFile(string relevantPath) {
    string path = root + relevantPath;

    ifstream file(path);
    if(!file.is_open()) {
        throw FileDoesNotExist();
    }
    string temp, res;
    while(getline(file, temp)) {
        res += temp;
        res += "\r\n";
    }
    return res;
}