/***
* Eric Lee
* elee144
* 2024 Fall CSE101 PA7
* Order.cpp
* Order main function
***/ 

#include <iostream>
#include <fstream>
#include <string>
#include "Dictionary.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return 1;
    }

    std::ofstream outputFile(argv[2]);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return 1;
    }

    Dictionary dict;

    std::string line;
    int value = 1;

    while (std::getline(inputFile, line)) {
        dict.setValue(line, value);
        value++;
    }

    outputFile << dict << endl;

    outputFile << dict.pre_string() << endl;

    outputFile.close();

    return 0;
}
