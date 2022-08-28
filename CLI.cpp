#include "CLI.h"
#include <iostream>

void CLI::start() {
    // int option;
    // do {
    //     printMenu();
    //     std::cin >> option;
    //     int result = 
    // } while ();




    printMenu();

    int option;
    std::cin >> option;
    switch (option)
    {
    case 1:
        /* code */
        break;
    
    case 2:
        /* code */
        break;
    
    case 3:
        /* code */
        break;
    
    case 4:
        /* code */
        break;
    
    case 5:
        /* code */
        break;
    
    case 6:
        /* code */
        break;

    default:
        break;
    }
}

void CLI::printMenu() {

    // maybe can be in an array of options... I did it for now.

    std::cout << "Welcome to the KNN Classifier Server. Please choose an option:" << std::endl;
    std::cout << "1. upload an unclassified csv data file" << std::endl;
    std::cout << "2. algorithm settings" << std::endl;
    std::cout << "3. classify data" << std::endl;
    std::cout << "4. display results" << std::endl;
    std::cout << "5. downlond results" << std::endl;
    std::cout << "6. display algorithm confusion matrix" << std::endl;
    std::cout << "7. exit" << std::endl;

}