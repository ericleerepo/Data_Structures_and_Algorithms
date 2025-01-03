/***
* Eric Lee
* elee144
* 2024 Fall CSE101 PA8
* WordFrequency.cpp
* WordFrequency main function
***/ 

#include<iostream>
#include<fstream>
#include<string>
#include <algorithm>

#include"Dictionary.h"

using namespace std;

#define MAX_LEN 300

int main(int argc, char * argv[]){

   int token_count, line_count;
   size_t begin, end, len;
   ifstream in;
   ofstream out;
   string line;
   string tokenBuffer;
   string token;
   //string delim = " "; 
   string delim = " \t\\\"\',<.>/?;:[{]}|`~!%@#$^&*()-_=+0123456789";

   // check command line for correct number of arguments
   if( argc != 3 ){
      cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
      return(EXIT_FAILURE);
   }

   // open files for reading and writing 
   in.open(argv[1]);
   if( !in.is_open() ){
      cerr << "Unable to open file " << argv[1] << " for reading" << endl;
      return(EXIT_FAILURE);
   }

   out.open(argv[2]);
   if( !out.is_open() ){
      cerr << "Unable to open file " << argv[2] << " for writing" << endl;
      return(EXIT_FAILURE);
   }

   // read each line of input file, then count and print tokens 
   line_count = 0;
   Dictionary dict;
   while (getline(in, line)) {
    line_count++;
    len = line.length();

    size_t begin = 0, end = 0;
    while (true) {
        // Find the first character not in delimiters
        begin = line.find_first_not_of(delim, end);
        if (begin == string::npos) break;  // No more tokens

        // Find the next delimiter after the token
        end = line.find_first_of(delim, begin);

        // Extract the token
        token = line.substr(begin, (end == string::npos ? string::npos : end - begin));

        // Process the token if it's non-empty
        if (token.length()>0) {
            std::transform(token.begin(), token.end(), token.begin(), ::tolower);
            //out << token << endl;  // Debugging statement to see each token
            if (dict.contains(token)) {
                dict.getValue(token) += 1;
            } else {
                dict.setValue(token, 1);
            }
        }
    }
}
   out << dict;

   // close files 
   in.close();
   out.close();

   return(EXIT_SUCCESS);
}
