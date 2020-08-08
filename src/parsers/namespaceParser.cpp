#include "namespaceParser.h"

using namespace std;

bool NamespaceParser::parseTag(InputParser* input, Element* el) {
    if (input->firstWord.compare("name") == 0) {

        // Check if we are overiding
        if (((Namespace*)el)->getName().compare("") != 0) {
            cout << "[Error] " << keyword << " " << ((Namespace*)el)->getName() << " duplicate name found";
            cout << " line number " << input->lineNumber << '\n';
            return false;
        }

        ((Namespace*)el)->setName(input->getTag());
        cout << "[Info] " << keyword << " name: " << ((Namespace*)el)->name << "\n";
        return true;
    }

    return ElementParser::parseTag(input, el);
}