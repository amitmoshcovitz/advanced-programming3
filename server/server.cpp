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
    const int sock_port = 5555;
    Server server(sock_port);
    server.run();
    close(sock_port);
    return 0;
}


Server::ServerThread::ServerThread(Server* server) {
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

void Server::ServerThread::sendToClient(int clientSock, string message) const {
    int sentBytes = send(clientSock, (message + END).c_str(), message.length() + 1, 0);
    if (sentBytes < 0) {
        perror("error sending to client");
    }
}

string Server::ServerThread::receiveFromClient(int clientSock) const {
    const int bufferSize = 1024;
    static char buffer[bufferSize];
    stringstream ss;
    do {
        buffer[0] = '\0';
        int receivedBytes = recv(clientSock, buffer, bufferSize, 0);
        if (receivedBytes < 0) {
            perror("error receiving from client");
        }
        ss << buffer;
    } while (string(buffer).find(END) == string::npos);
    string str = ss.str();
    str.erase(str.length() - 1);
    return str;
}

void Server::ServerThread::run() {
    running = true;
    while (!(isStopped && server->clientSocks.empty())) {
        int clientSock = acceptClient();
        reset();
        if (clientSock <= 0) {
            sleep(2);
            continue;
        }
        handleClient(clientSock);
    }
    running = false;
    terminate();
}

void Server::ServerThread::reset() {
    k = defaultK;
    metric = defaultMetric;
    testPoints.clear();
    points.clear();
    testResults.clear();
}

bool Server::ServerThread::handleTrainUpload(string file) {
    stringstream ss(file);
    string line;
    getline(ss, line);
    string pointStr = line.substr(0, line.find_last_of(','));
    if (!isPoint(pointStr)) {
        return false;
    }
    Point point(pointStr);
    int dimension = point.getDimension();
    points[point] = line.substr(line.find_last_of(',') + 1);
    while (getline(ss, line)) {
        string pointStr = line.substr(0, line.find_last_of(','));
        if (!isPoint(pointStr)) {
            return false;
        }
        Point point(pointStr);
        if (point.getDimension() != dimension) {
            return false;
        }
        points[point] = line.substr(line.find_last_of(',') + 1);
    }
    return true;
}

bool Server::ServerThread::handleTestUpload(string file) {
    stringstream ss(file);
    string line;
    int dimension = points.begin()->first.getDimension();
    while (getline(ss, line)) {
        string pointStr = line;
        if (!isPoint(pointStr)) {
            return false;
        }
        Point point(pointStr);
        if (point.getDimension() != dimension) {
            return false;
        }
        testPoints.push_back(point);
    }
    return true;
}

void Server::ServerThread::processTest() {
    for (Point point : testPoints) {
        testResults.push_back(classify(point, points, metric, k));
    }
}

void Server::ServerThread::handleClient(int clientSock) {
    
}

bool Server::ServerThread::isRunning() {
    return running;
}

bool Server::ServerThread::isValid(const Point &point) const {
    return point.getDimension() == points.begin()->first.getDimension();
}

void Server::ServerThread::start() {
    isStopped = false;
    #ifdef WIN32
    HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Server::ServerThread::run, this, 0, NULL);
    #else
    pthread_t thread;
    pthread_create(&thread, NULL, (void* (*)(void*))&Server::ServerThread::run, this);
    pthread_join(thread, NULL);
    #endif
}

void Server::ServerThread::stop() {
    isStopped = true;
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

bool Server::acceptClient() {
    struct sockaddr_in clientSin;
    #ifdef WIN32
    int addrLen = sizeof(clientSin);
    #else
    unsigned int addrLen = sizeof(clientSin);
    #endif
    int clientSock = accept(sock, (struct sockaddr *) &clientSin, &addrLen);
    if (clientSock < 0) {
        perror("error accepting client");
        return false;
    }
    clientSocks.push(clientSock);
    return true;
}

void Server::run() {
    int threadCount = 10;
    ServerThread* threads[threadCount];
    for (int i = 0; i < threadCount; i++) {
        threads[i] = &ServerThread(this);
        threads[i]->start();
    }
    while (acceptClient());
    for (int i = 0; i < threadCount; i++) {
        threads[i]->stop();
    }
    for (int i = 0; i < threadCount; i++) {
        while (threads[i]->isRunning()) {
            sleep(2);
        }
    }
}
