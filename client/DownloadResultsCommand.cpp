#include "DownloadResultsCommand.h"
#include <iostream>
#include <fstream>
#include <stdio.h>



void DownloadResultsCommand::execute() const {
    dio.write("04");
    std::ofstream file 
    std::cout << dio.read() << std::endl;
}