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

using namespace std;

int Client::sendFile(std::string filePath, int socket) {
    ifstream file(filePath);
    if (!file.is_open())
        return ERROR;

    string line;
    string fullStr;
    while (getline(file, line)) {
        fullStr += line + "\n";
    }
    fullStr += END;
    int sent_bytes = send(socket, fullStr.c_str(), fullStr.length(), 0);
    if (sent_bytes < 0) {
        file.close();
        return ERROR;
    }
    file.close();
    return SUCCESS;
}

int Client::getLine(int socket, char* buffer, int bufferSize) {
    int read_bytes = recv(socket, buffer, bufferSize, 0);
    if(read_bytes < 0)
        return ERROR;
    return SUCCESS;
}

int Client::writeFileFromSocket(int socket, std::string filePath) {
    ofstream file(filePath);
    if (!file.is_open())
        return ERROR;
    char buffer[Client::DEFAULT_BUFFER_SIZE];
    int bufferSize = Client::DEFAULT_BUFFER_SIZE;
    std::string str = "";
    std::string strBuffer;
    do {
        int result = Client::getLine(socket, buffer, bufferSize);
        if (result < 0) {
            file.close();
            return ERROR;
        } 
        strBuffer = string(buffer);
        str += buffer;    
    } while(strBuffer.find(END) == string::npos);
    str = str.substr(0, str.length() - 1);
    file << str;
    file.close();
    return SUCCESS;
}


int main(int argc, char const *argv[]) {

    if (argc < 3)
        perror("Incorrect number of arguments");

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

    Client user;
    int resultSending = user.sendFile(argv[1], sock);
    if (resultSending < 0)
        perror("An error occured sending the file to the server");

    int resultWriting = user.writeFileFromSocket(sock, argv[2]);
    if (resultWriting < 0)
        perror("An error occured writing the results to a file");

    close(sock);
    return 0;
}