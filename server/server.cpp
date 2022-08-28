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
    Server server(sock_port, k, Point::EUCLIDEAN);
    server.loadClassified(argv[1]);
    server.run();
    close(sock_port);
    return 0;
}


Server::Server(int port, int k, Point::DistanceMetric metric) {
    this->k = k;
    this->metric = metric;
    const int server_port = port;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("error creating socket");
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);

    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error binding socket");
    }

    if (listen(sock, 5) < 0) {
        perror("error listening to a socket");
    }
    this->sock = sock;
}

Server::~Server() {
    close(sock);
}

int Server::getSock() const {
    return sock;
}

int Server::acceptClient() const {
    struct sockaddr_in clientSin;
    #ifdef WIN32
    int addrLen = sizeof(clientSin);
    #else
    unsigned int addrLen = sizeof(clientSin);
    #endif

    if (listen(sock, 5) < 0) {
        perror("error listening to a socket");
    }
    
    int clientSock = accept(sock, (struct sockaddr *) &clientSin,  &addrLen);

    if (clientSock < 0) {
        perror("error accepting client");
    }

    return clientSock;
}

void Server::sendToClient(int clientSock, const char* buffer, int bufferSize) const {
    int sentBytes = send(clientSock, buffer, bufferSize, 0);
    if (sentBytes < 0) {
        perror("error sending to client");
    }
}

bool Server::receiveFromClient(int clientSock, char* buffer, int bufferSize) const {
    int readBytes = recv(clientSock, buffer, bufferSize, 0);
    if (readBytes < 0) {
        perror("error reading from client");
    }
    return string(buffer).find(END) == string::npos;
}

void Server::loadClassified(string fileName) {
    points = decryptClassifiedFile(fileName);
    if (points.empty()) {
        cout << "Error loading classified file" << endl;
        exit(1);
    }

    if (points.size() < k) {
        cout << "Error: k is greater than the number of points in the classified file" << endl;
        exit(1);
    }
}

void Server::run() {
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

bool Server::isValid(const Point &point) const {
    return point.getDimension() == points.begin()->first.getDimension();
}
