#ifndef CLASSIFIERPARSER
#define CLASSIFIERPARSER
#include "namespaceParser.h"
using namespace std;

class ClassifierParser: public NamespaceParser {
    protected:
        bool parseTag(InputParser* inputParser, Element * el);
};
#endif