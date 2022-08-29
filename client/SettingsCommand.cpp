#include "SettingsCommand.h"
#include <iostream>

void SettingsCommand::execute() const {
    dio.write("02");
    std::cout << dio.read() << std::endl;
    std::string userSettings;
    do {
        std::cin >> userSettings;
        dio.write(userSettings);
        if (dio.read().compare("success"))
        {
            std::cout << userSettings << std::endl;
            std::cout << "Please try again:" << std::endl;
        } else {
            break;
        }
    } while(true);
}