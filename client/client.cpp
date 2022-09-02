#include <iostream>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include "client.h"
#include <sstream>
#include <thread>
#define INPUT_EXPECTED "input expected"

using namespace std;

void Client::sendLine(string line) {
    socketIO.write(line);
}

int Client::sendFile(std::string filePath) {
    ifstream file(filePath);
    if (!file.is_open())
        return ERROR;

    string line;
    string fullStr;
    getline(file, fullStr);
    while (getline(file, line)) {
        fullStr += "\n" + line;
    }
    socketIO.write(fullStr);
    file.close();
    return SUCCESS;
}

void Client::writeFile(std::string filePath, string str) {
    ofstream file(filePath);
    if (!file.is_open())
        return;
    stringstream ss(str);
    string line;
    while (getline(ss, line)) {
        file << line << endl;
    }
    file.close();
}

string Client::getInput() const {
    return socketIO.read();
}

Client::Client(int socket) {
    socketIO = SocketIO();
    socketIO.setSocket(socket);
}

int main(int argc, char const *argv[]) {
    const char* ip_address = "127.0.0.1";
    const int port_no = 5555;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("error creating socket");
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(port_no);

    if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error connecting to server");
    }

    Client user(sock);
    string line;
    string input;
    string last_input;
    thread* downloadThread = nullptr;
    while (true) {
        input = user.getInput();
        if (input == "Goodbye!") {
            close(sock);
            return 0;
        }
        if (input == INPUT_EXPECTED) {
            getline(cin, line);
            if (last_input.rfind("Please upload", 0) == 0) {
                if (user.sendFile(line) == ERROR) {
                    user.sendLine("a,a");
                }
            } else if (last_input.rfind("Please enter", 0) == 0) {
                user.sendLine(line);
                if (downloadThread) {
                    downloadThread->join();
                    delete downloadThread;
                }
                downloadThread = new thread(&Client::writeFile, &user, line, user.getInput());
                user.sendLine("done");
            } else {
                user.sendLine(line);
            }
        } else {
            cout << input;
            user.sendLine("done");
        }
        last_input = input;
    }
    downloadThread->join();
    delete downloadThread;
    return 0;
}