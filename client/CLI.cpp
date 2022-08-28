#include "CLI.h"
#include <iostream>
#include "CLI.h"

void CLI::start() {
    int option = getUserDecision();
    commands[option]->execute();
}

int CLI::getUserDecision() {
    int option = -1;
    do {
        printMenu();
        std::cin >> option;
        if (option == -1)
            std::cout << "Invalid number, please enter a number between 1 and " + 8 << std::endl;
    } while(option == -1);
    return option;
}

void CLI::printMenu() {
    std::cout << "Welcome to the KNN Classifier Server. Please choose an option:" << std::endl;
    for (int i = 1; i <= NUM_OF_OPTIONS; i++) {
        std::cout << i + ". " + commands[i]->getDescription() << std::endl;
    }
}