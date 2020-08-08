#include "templateParser.h"

using namespace std;

bool TemplateParser::parse(InputParser* input) {
    cout << "[Info] keyword " << keyword << '\n';
    int startTabs = input->numTabs;
    while (input->readNextLine()) {
        // identify comments 
        cout << "[Info] first word " << input->firstWord << '\n';
        if (input->shouldSkip()) {
            cout << "skipping\n";
            continue;
        // parse element
        } else if (input->firstWord.compare(keyword) == 0) {
            cout << "parsing element " << keyword << '\n';
            parseElement(input);
            return true;
        // it ended
        } else if (input->numTabs <= startTabs) {
            cerr << "[Error] didn't parse " << keyword << " \n";
            break;
        }
    }
    return false;
}

Element *TemplateParser::parseElement(InputParser* input) {
    int numTabs = input->numTabs;
    Element * el = createElement();
    while(input->readNextLine()) {
        // check if line is valid
        if (input->shouldSkip()) {
            continue;
        } else if (input->numTabs < numTabs) {
            // TODO something?
            break;
        // check if value is readable
        } else if (!parseTag(input, el)) {
            // TODO log error
            cerr << "[Error] Could not recognize tag " << input->firstWord << " line " << input->lineNumber << "\n";
        }
    }
    return el;
}