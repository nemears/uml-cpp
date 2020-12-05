#include "headers/inputParser.h"

bool InputParser::readNextLine() {
    
    if (resetLastLine) {
        resetLastLine = false;
        return true;
    }
    if (lastLine) {
        return false;
    }
    currentLine.clear();
    firstWord.clear();

    if (lineNumber == 0) {
        if (!getline(file,currentLine)) {
            return false;
        }
    } else {
        currentLine = nextLine;
    }

    lineNumber++;

    cout << "[Info] " << lineNumber << ": " << currentLine;

    //get first word
    int i = 0;
    int firstChar = currentLine.length();
    int lastChar = currentLine.length();
    char currChar= currentLine[0];
    while (i < currentLine.length()) {
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
    firstWord = currentLine.substr(firstChar,lastChar-firstChar);
    cout << ", first word: <" << firstWord << ">\n";

    if (getline(file,nextLine)) {
        numTabs = nextLineTabs;
        currChar= nextLine[0];
        int tabCount = 0;
        i = 0;
        while (currChar == ' ' || currChar == '\t') {
            i++;
            if (currChar == ' ') {
                tabCount++;
            } else {
                tabCount += 2; // TODO check
            }
            currChar = nextLine[i];
        }
        nextLineTabs = tabCount;

        return true;
    } else {
        lastLine = true;
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

// This function expects that it is a simple tag within quotes
string InputParser::getTag() {
    int colon = currentLine.find_first_of(':');
    
    int i = colon + 1;
    char currChar = currentLine[i];
    int firstQuote = currentLine.length();
    int lastQuote = currentLine.length();
    while (i < currentLine.length()) {
        if (currChar == '\"') {
            if (i < firstQuote) {
                firstQuote = i;
            } else {
                lastQuote = i;
            }
        }
        i++;
        currChar = currentLine[i];
    }

    return currentLine.substr(firstQuote + 1, lastQuote - firstQuote - 1);
}

// this function gets a string of everything after the : and with greater indentation
string InputParser::getElement() {
    int colon = currentLine.find_first_of(':');

    // get rest of line
    string ret = currentLine.substr(colon + 1, currentLine.length() - colon - 1);

    // TODO do the tabs thing

    return ret;
}

string InputParser::getFileName() {
    return fileName;
}

bool InputParser::resetLine() {
    if (lineNumber >= 2) {
        resetLastLine = true;
        return true;
    }
    return false;
}