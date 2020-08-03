#ifndef INPUTPARSER
#define INPUTPARSER
#include <fstream>
#include <string>
#include <string.h>
#include <iostream>
#include <algorithm>
using namespace std;

class InputParser {

    private:

    fstream file;
    string fileName;
    char* tokenizedLine;

    public:

    string currentLine;
    string currTag;
    string firstWord;
    int lineNumber;
    int numTabs;

    InputParser(string f) {
        fileName = f;
        file.open(f);
        lineNumber = 0;
    };

    bool readNextLine();
    bool shouldSkip();
    string getTag();
};
#endif