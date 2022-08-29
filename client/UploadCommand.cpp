#include "UploadCommand.h"
#include <iostream>
#include <sstream>
#include<fstream>
#include<string>

void UploadCommand::execute() const {
    dio.write("01");
    std::cout << dio.read() << std::endl;

    std::string path = nullptr;
    bool doesFileExist = false, shouldExit = false;
    std::string userChoice;
    while (!doesFileExist && !shouldExit)
    {
        std::cin >> path;
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cout << "Invalid file path, would you like to try again? (y / n or any other key)" << std::endl;
            std::cin >> userChoice;
            if (userChoice.compare("y") || userChoice.compare("Y"))
                shouldExit = true;
        } else {
            doesFileExist = true;
            file.close();
        }
    }
    sendFile(path);
    dio.write("01");
    std::cout << dio.read() << std::endl;
}

int UploadCommand::sendFile(const std::string filePath) const{
    std::ifstream file(filePath);
    if (!file.is_open())
        return ERROR;

    std::string line;
    std::string fullStr;
    while (getline(file, line)) {
        fullStr += line + "\n";
    }
    fullStr += END;
    dio.write(fullStr);
    file.close();
    return SUCCESS;
}