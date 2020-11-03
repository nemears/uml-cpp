#ifndef CLASSIFIERPARSER
#define CLASSIFIERPARSER
#include "namespaceParser.h"
#include "propertyParser.h"
#include "../../uml/headers/classifier.h"
using namespace std;

class ClassifierParser: public NamespaceParser {
    protected:
        bool parseTag(InputParser* inputParser, Element * el);
};
#endif