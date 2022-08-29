#include "DisplayResultsCommand.h"
#include <iostream>

void DisplayResultsCommand::execute() const {
    dio.write("04");
    std::cout << dio.read() << std::endl;
}