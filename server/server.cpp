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
#include "../point.h"
#include <map>
#include <fstream>
#include "../fileHandler.h"
#include "../algorithms.h"
#include "server.h"
#define END '\u0003'

using namespace std;

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        return -1;
    }
    const int sock_port = 5555;
    int k = stoi(argv[2]);
    if (k < 1) {
        cout << "k must be greater than 0" << endl;
        return -1;
    }
    Server server(sock_port);
    server.run();
    close(sock_port);
    return 0;
}


Server::ServerThread::ServerThread(Server* server) {
    this->k = 5;
    this-> server = server;
}

Server::ServerThread::~ServerThread() {}

int Server::ServerThread::acceptClient() const {
    struct sockaddr_in clientSin;
    #ifdef WIN32
    int addrLen = sizeof(clientSin);
    #else
    unsigned int addrLen = sizeof(clientSin);
    #endif

    if (server->clientSocks.empty()) {
        return -1;
    }

    int clientSock = server->clientSocks.front();
    server->clientSocks.pop();
    return clientSock;
}

void Server::ServerThread::sendToClient(int clientSock, const char* buffer, int bufferSize) const {
    int sentBytes = send(clientSock, buffer, bufferSize, 0);
    if (sentBytes < 0) {
        perror("error sending to client");
    }
}

bool Server::ServerThread::receiveFromClient(int clientSock, char* buffer, int bufferSize) const {
    int readBytes = recv(clientSock, buffer, bufferSize, 0);
    if (readBytes < 0) {
        perror("error reading from client");
    }
    return string(buffer).find(END) == string::npos;
}

void Server::ServerThread::run() {
    char buffer[1 << 12];
    int expectedDataLen = sizeof(buffer);
    while (true) {
        buffer[0] = '\0';
        stringstream ss;
        int clientSock = acceptClient();
        if (clientSock <= 0) {
            return;
        }
        while (receiveFromClient(clientSock, buffer, expectedDataLen)) {
            ss << buffer;
        }
        ss << buffer;
        string line;
        string ans = "";
        while (getline(ss, line)) {
            if (line[0] == END) break;
            if (!isPoint(line)) {
                ans += "Error: invalid point\n";
                continue;
            }
            Point point(line);
            if (!isValid(point)) {
                ans += "Invalid point\n";
                continue;
            }
            string cls = classify(point, points, metric, k);
            ans += cls + "\n";
        }
        ans += END;
        sendToClient(clientSock, ans.c_str(), ans.length());
        close(clientSock);
    }
}

bool Server::ServerThread::isValid(const Point &point) const {
    return point.getDimension() == points.begin()->first.getDimension();
}

Server::Server(int port) {
    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    clientSocks = queue<int>();
    if (sock < 0) {
        perror("error opening socket");
    }
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error binding socket");
    }
    listen(sock, 1);
}
