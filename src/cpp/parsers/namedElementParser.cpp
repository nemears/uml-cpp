#include "headers/namedElementParser.h"

bool NamedElementParser::parseTag(InputParser* input, Element* el) {
    if (input->firstWord.compare("name") == 0) {

        // Check if we are overiding
        if (((NamedElement*)el)->getName().compare("") != 0) {
            cout << "[Error] " << keyword << " " << ((NamedElement*)el)->getName() << " duplicate name found";
            cout << " line number " << input->lineNumber << '\n';
            return false;
        }
        ((NamedElement*)el)->setName(input->getTag());
        cout << "[Info] " << keyword << " name: " << ((NamedElement*)el)->getName() << "\n";
        return true;
    }

    return ElementParser::parseTag(input, el);
}