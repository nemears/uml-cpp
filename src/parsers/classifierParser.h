#ifndef CLASSIFIERPARSER
#define CLASSIFIERPARSER
#include "namespaceParser.h"
#include "propertyParser.h"
#include "../uml/classifier.h"
using namespace std;

class ClassifierParser: public NamespaceParser {
    protected:
        bool parseTag(InputParser* inputParser, Element * el);
};
#endif