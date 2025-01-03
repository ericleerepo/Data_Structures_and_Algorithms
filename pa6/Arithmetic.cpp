/***
* Eric Lee
* elee144
* 2024 Fall CSE101 PA6
* Arithmetic.cpp
* Arithmetic main function
***/ 

#include <iostream>
#include <fstream>
#include <string>
#include "BigInteger.h"

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

    std::string line1, line2, line3;
    getline(inputFile, line1);  
    getline(inputFile, line2);  
    getline(inputFile, line3);  
    inputFile.close();

    BigInteger A(line1);
    BigInteger B(line3);

    std::ofstream outputFile(argv[2]);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return 1;
    }

    outputFile << A << endl << endl;
    outputFile << B << endl << endl;

    outputFile << A+B << endl << endl;
    outputFile << A-B << endl << endl;
    outputFile << A-A << endl << endl;

    outputFile << 3*A-2*B << endl << endl;
    outputFile << A*B << endl << endl;

    outputFile << A*A << endl << endl;
    outputFile << B*B << endl << endl;

    
    BigInteger four = A*A*A*A;
    BigInteger five = B*B*B*B*B;
    outputFile << 9*four+16*five<< endl;

    outputFile.close();

    return 0;
}






