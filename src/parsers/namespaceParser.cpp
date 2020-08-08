#include "namespaceParser.h"

using namespace std;

bool NamespaceParser::parseTag(InputParser* input, Element* el) {
    if (input->firstWord.compare("name") == 0) {
        //cout << nmspc->tempid << '\n';
        ((Namespace*)el)->setName(input->getTag());
        //cout << input->getTag() << '\n';
        //nmspc->name = input->getTag();
        cout << "[Info] " << keyword << " name: " << ((Namespace*)el)->name << "\n";
        return true;
    }

    return false;
}