#include "projectParser.h"

using namespace std;

Element ProjectParser::parseElement(InputParser* input) {
    int numTabs = input->numTabs;
    Project ret;
    while(input->readNextLine()) {
        // check if line is valid
        if (input->shouldSkip()) {
            continue;
        } else if (input->numTabs < numTabs) {
            // TODO something?
            break;
        // check if value is readable
        } else if (!parseTag(input, ret)) {
            // TODO log error
            cerr << "[Error] Could not recognize tag " << input->firstWord << " line " << input->lineNumber << "\n";
        }
    }
    return ret;
}

bool ProjectParser::parseTag(InputParser* input, Project el) {
    if (input->firstWord.compare("name")) {
        el.setName(input->firstWord);
        cout << "[Info] Setting name for project " << el.getName() << " to " << input->firstWord << "\n";
        return true;
    }

    return false;
}