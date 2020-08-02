#include "inputParser.h"

using namespace std;

bool InputParser::readNextLine() {
    
    currentLine.clear();
    firstWord.clear();

    if (getline(file,currentLine)) {

        lineNumber++;

        cout << "[Info] " << lineNumber << ": " << currentLine;

        //get first word
        int i = 0;
        int firstChar = currentLine.length();
        int lastChar = currentLine.length();
        char currChar= currentLine[0];
      //  cout << "\n\n";
        while (i < currentLine.length()) {
            //cout << currChar << '\n';
            if (currChar != ' ' && currChar != '\t' && currChar != ':') {
                if (i < firstChar) {
                    // specific yaml syntax check
                    if (currChar != '-') {
                        firstChar = i;
                    }
                }
            }
            else if (i > firstChar) {
                lastChar = i;
                break;
            }
            i++;
            currChar = currentLine[i];
        }
        //cout << "first char " << firstChar << ", last char " << lastChar;
        firstWord = currentLine.substr(firstChar,lastChar-firstChar);
        cout << ", first word: <" << firstWord << ">\n";

        //count tabs
        currChar= currentLine[0];
        int tabCount = 0;
        i = 0;
        while (currChar == ' ' || currChar == '\t') {
            i++;
            if (currChar == ' ') {
                tabCount++;
            } else {
                tabCount += 2; // TODO check
            }
            currChar = currentLine[i];
        }

        return true;
    }

    return false;
}

bool InputParser::shouldSkip() {
    if (firstWord[0] == '#') {
        return true;
    } else if (lineNumber == 1) { // TODO case where first lines are all comments
        if (firstWord.compare("---")) {
            return true;
        }
    }
    return false;
}