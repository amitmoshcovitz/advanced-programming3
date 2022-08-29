#include "StandardIO.h"
#include <iostream>

using namespace std;

void StandardIO::write(std::string message) const {
    std::cout << message << endl;
}

string StandardIO::read() const {
    string str;
    getline(cin, str);
    return str;
}