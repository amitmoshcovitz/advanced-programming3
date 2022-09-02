#include "SocketIO.h"
#ifdef WIN32
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#endif
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "point.h"
#include <map>
#include <fstream>
#include "fileHandler.h"
#include "algorithms.h"
#include "server/server.h"
#include "DefaultIO.h"
#define END '\u0003'

using namespace std;

SocketIO::SocketIO() {
    socket = 0;
}

void SocketIO::write(string message) const {
    int sentBytes = send(socket, (message + END).c_str(), message.length() + 2, 0);
    if (sentBytes < 0) {
        perror("error sending to client");
    }
}

string SocketIO::read() const {
    const int bufferSize = 10000;
    char buffer[bufferSize];
    stringstream ss;
    do {
        buffer[0] = '\0';
        int receivedBytes = recv(socket, buffer, bufferSize, 0);
        if (receivedBytes < 0) {
            std::cout << "error receiving from client" << std::endl;
            return "";
        }
        ss << buffer;
    } while (string(buffer).find(END) == string::npos);
    string str = ss.str();
    str.erase(str.length() - 1);
    return str;
}

void SocketIO::setSocket(int sock) {
    socket = sock;
}

int SocketIO::getSocket() const {
    return socket;
}