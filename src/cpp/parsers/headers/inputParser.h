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
        bool lastLine;

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
};
#endif