#include "inputParser.h"

using namespace std;

bool InputParser::readNextLine() {
    
    currentLine.clear();

    if (getline(file,currentLine)) {

        lineNumber++;

        //get first word
        int n = currentLine.length();
        tokenizedLine = (char*)realloc(tokenizedLine, n*sizeof(char));
        firstWord = strtok(tokenizedLine, " :");

        //count tabs
        char currChar= currentLine[0];
        int tabCount = 0;
        int i = 0;
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