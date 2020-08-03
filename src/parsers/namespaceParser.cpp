#include "namespaceParser.h"

using namespace std;

bool NamespaceParser::parseTag(InputParser* input, Namespace el) {
    if (input->firstWord.compare("name") == 0) {
        el.setName(input->getTag());
        cout << "[Info] Namespace name: " << el.getName() << "\n";
        return true;
    }

    return false;
}