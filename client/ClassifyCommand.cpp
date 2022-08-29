#include "ClassifyCommand.h"
#include <iostream>

void ClassifyCommand::execute() const {
    dio.write("03");
    std::cout << dio.read() << std::endl;
}