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
#include "../StandardIO.h"
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
    this->server = server;
    commands = vector<Command*>(7);
    commands[0] = new UploadCommand(&dio, this);
    commands[1] = new AlgorithmSettingsCommand(&dio, this);
    commands[2] = new DetectCommand(&dio, this);
    commands[3] = new DisplayCommand(&dio, this);
    commands[4] = new DownloadCommand(&dio, this);
    commands[5] = new AnalyzeCommand(&dio, this);
    commands[6] = new ExitCommand(&dio, this);
}

Server::ServerThread::~ServerThread() {
    for (Command* command : commands) {
        delete command;
    }
}

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

void Server::ServerThread::run() {
    #if IO == 0
    running = true;
    while (!(isStopped && server->clientSocks.empty())) {
        int clientSock = acceptClient();
        reset();
        if (clientSock <= 0) {
            sleep(2);
            continue;
        }
        dio.setSocket(clientSock);
        handleClient(clientSock);
    }
    running = false;
    terminate();
    #else
    reset();
    handleClient(0);
    #endif
}

void Server::ServerThread::reset() {
    k = defaultK;
    metric = defaultMetric;
    testPoints.clear();
    points.clear();
    testResults.clear();
}

bool Server::ServerThread::handleTrainUpload(string file) {
    points.clear();
    #if IO == 0
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
    #else
    points = decryptClassifiedFile(file);
    return true;
    #endif
}

bool Server::ServerThread::handleTestUpload(string file) {
    testPoints.clear();
    #if IO == 0
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
    #else
    testPoints = decryptUnclassifiedFile(file);
    return true;
    #endif
}


void Server::ServerThread::handleClient(int clientSock) {
    int choice;
    int commandCount = commands.size();
    do {
        dio.write("Welcome to the KNN Classifier Server. Please choose an option:");
        for (int i = 0; i < commandCount; i++) {
            dio.write(to_string(i + 1) + " " + commands[i]->getDescription());
        }
        string input = dio.read();
        if (!isInteger(input)) {
            dio.write("Invalid input.");
            continue;
        }
        choice = stoi(input);
        if (choice > commandCount|| choice <= 0) {
            dio.write("Invalid input.");
            continue;
        }
        commands[choice - 1]->execute();
    } while(choice != commandCount);
}

bool Server::ServerThread::isRunning() {
    return running;
}

bool Server::ServerThread::isValid(const Point &point) const {
    return point.getDimension() == points.begin()->first.getDimension();
}

void Server::ServerThread::start() {
    isStopped = false;
    thread t(&ServerThread::run, this);
    t.join();
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
    #if IO == 0
    int threadCount = 10;
    vector<ServerThread> threads;
    for (int i = 0; i < threadCount; i++) {
        threads.push_back(ServerThread(this));
        threads[i].start();
    }
    while (acceptClient());
    for (int i = 0; i < threadCount; i++) {
        threads[i].stop();
    }
    for (int i = 0; i < threadCount; i++) {
        while (threads[i].isRunning()) {
            sleep(2);
        }
    }
    #else
    ServerThread thread(this);
    thread.run();
    #endif
}

Server::ServerThread::UploadCommand::UploadCommand(DefaultIO* dio, ServerThread* serverThread) {
    this->serverThread = serverThread;
    this->dio = dio;
    this->description = "upload an unclassified csv data file";
}

void Server::ServerThread::UploadCommand::execute() const {
    dio->write("Please upload your local train CSV file.");
    string file = dio->read();
    if (!serverThread->handleTrainUpload(file)) {
        dio->write("Upload failed.");
        return;
    }
    dio->write("Upload complete.");
    dio->write("Please upload your local test CSV file.");
    file = dio->read();
    if (!serverThread->handleTestUpload(file)) {
        dio->write("Upload failed.");
        return;
    }
    dio->write("Upload complete.");
}

Server::ServerThread::AlgorithmSettingsCommand::AlgorithmSettingsCommand(DefaultIO* dio, ServerThread* serverThread) {
    this->serverThread = serverThread;
    this->dio = dio;
    this->description = "algorithm settings";
}

void Server::ServerThread::AlgorithmSettingsCommand::execute() const {
    dio->write("The current KNN parameters are: K = " + to_string(serverThread->k) + ", distance metric = " + Point::toString(serverThread->metric));
    string str = dio->read();
    string k = str.substr(0, str.find(' '));
    dio->write(k);
    if (!isInteger(k)) {
        dio->write("Invalid value for K");
        return;
    }
    serverThread->k = stoi(k);
    string metricStr = str.substr(str.find(' ') + 1);
    Point::DistanceMetric metric = Point::stringToMetric(metricStr);
    if (metric == Point::DistanceMetric::ERR) {
        dio->write("Invalid value for distance metric");
        return;
    }
    serverThread->metric = metric;
    dio->write("Updated KNN parameters.");
}

Server::ServerThread::DetectCommand::DetectCommand(DefaultIO* dio, ServerThread* serverThread) {
    this->serverThread = serverThread;
    this->dio = dio;
    this->description = "classify data";
}

void Server::ServerThread::DetectCommand::execute() const {
    for (Point point : serverThread->testPoints) {
        serverThread->testResults.push_back(classify(point, serverThread->points, serverThread->metric, serverThread->k));
    }
}

Server::ServerThread::DisplayCommand::DisplayCommand(DefaultIO* dio, ServerThread* serverThread) {
    this->serverThread = serverThread;
    this->dio = dio;
    this->description = "display results";
}

void Server::ServerThread::DisplayCommand::execute() const {
    int length = serverThread->testResults.size();
    for (int i = 0; i < length; i++) {
        dio->write(to_string(i + 1) + " " + serverThread->testResults[i]);
    }
    dio->write("Done.");
}

Server::ServerThread::DownloadCommand::DownloadCommand(DefaultIO* dio, ServerThread* serverThread) {
    this->serverThread = serverThread;
    this->dio = dio;
    this->description = "download results";
}

void Server::ServerThread::DownloadCommand::execute() const {
    #if IO == 0
    int length = serverThread->testResults.size();
    for (int i = 0; i < length; i++) {
        dio->write(to_string(i + 1) + " " + serverThread->testResults[i]);
    }
    dio->write("Done.");
    #else
    dio->write("Please enter the file path to download the results to.");
    string path = dio->read();
    ofstream file(path);
    if (!file.is_open()) {
        dio->write("Failed to open file.");
        return;
    }
    int length = serverThread->testResults.size();
    for (int i = 0; i < length; i++) {
        file << to_string(i + 1) + " " + serverThread->testResults[i] << endl;
    }
    file.close();
    #endif
}


Server::ServerThread::AnalyzeCommand::AnalyzeCommand(DefaultIO* dio, ServerThread* serverThread) {
    this->serverThread = serverThread;
    this->dio = dio;
    this->description = "display algorithm confusion matrix";
}

void Server::ServerThread::AnalyzeCommand::execute() const {
    vector<string> types;
    for (auto point : serverThread->points) {
        if (indexOf(types, point.second) == -1) {
            types.push_back(point.second);
        }
    }
    vector<vector<int>> confusionMatrix;
    for (auto type : types) {
        vector<int> row;
        for (auto type2 : types) {
            row.push_back(0);
        }
        confusionMatrix.push_back(row);
    }
    for (auto point : serverThread->points) {
        string trueType = point.second;
        string predictType = classify(point.first, serverThread->points, serverThread->metric, serverThread->k);
        confusionMatrix[indexOf(types, trueType)][indexOf(types, predictType)]++;
    }
    for (vector<int>& row : confusionMatrix) {
        int amount = 0;
        for (int i : row) {
            amount += i;
        }
        vector<pair<int, int>> modulus;
        for (int i = 0; i < row.size(); i++) {
            modulus.push_back(pair<int, int>(row[i] % amount, i));
        }
        for (int i = 0; i < row.size(); i++) {
            row[i] *= 100;
            row[i] /= amount;
        }
        int addNeeded = 100;
        for (int i : row) {
            addNeeded -= i;
        }
        kthSmallest<pair<int,int>>(modulus, addNeeded, [](pair<int, int> a, pair<int, int> b) {
            return a.first > b.first;
        });
        for (int i = 0; i < addNeeded; i++) {
            row[modulus[i].second]++;
        }
    }
    string output = "Confusion matrix:\n";
    for (int i = 0; i < types.size(); i++) {
        output += "\t" + types[i];
    }
    output += "\n";
    for (int i = 0; i < types.size(); i++) {
        output += types[i] + "\t";
        for (int j = 0; j < types.size(); j++) {
            output += to_string(confusionMatrix[i][j]) + "%\t";
        }
        output += "\n";
    }
    output += "K = " + to_string(serverThread->k) + ", distance metric = " + Point::toString(serverThread->metric);
    dio->write(output);
}

Server::ServerThread::ExitCommand::ExitCommand(DefaultIO* dio, ServerThread* serverThread) {
    this->serverThread = serverThread;
    this->dio = dio;
    this->description = "exit";
}

void Server::ServerThread::ExitCommand::execute() const {
    
}