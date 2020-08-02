#include "templateParser.h"

using namespace std;

bool TemplateParser::parse(InputParser* input) {
    int startTabs = input->numTabs;
    while (input->readNextLine()) {
        // identify comments 
        if (input->shouldSkip()) {
            cout << "skipping\n";
            continue;
        // parse element
        } else if (input->firstWord.compare(keyword)) {
            cout << "parsing element " << keyword << '\n';
            element = parseElement(input);
            return true;
        // it ended
        } else if (input->numTabs <= startTabs) {
            cout << "[Info] Done parsing\n";
            break;
        }
    }
    return false;
}