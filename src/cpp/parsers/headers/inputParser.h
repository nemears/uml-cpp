#ifndef INPUTPARSER
#define INPUTPARSER
#include <fstream>
#include <string>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <sstream>
using namespace std;

class InputParser {

    private:
        fstream file;
        string fileName;
        bool lastLine;
        bool resetLastLine;

    public:
        string currentLine;
        string nextLine;
        string currTag;
        string firstWord;
        int lineNumber;
        int numTabs;
        int nextLineTabs;
        InputParser(string f) {
            fileName = f;
            file.open(f);
            lineNumber = 0;
            lastLine = false;
        };
        ~InputParser() {
            file.close();
            lineNumber = 0;
        }
        bool readNextLine();
        bool shouldSkip();
        string getTag();
        string getElement();
        string getFileName();
        bool resetLine();
        class InvalidTagException: public exception {
            string msg;
            public:
            InvalidTagException(const string& fileName, int lineNumber) 
                :msg(string("Could not find string value for tag, no quotes found in file " + fileName + " at line number " + to_string(lineNumber))) {};
            virtual const char* what() const throw() {
                return msg.c_str();
            }
        };
};
#endif