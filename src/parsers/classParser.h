#ifndef CLASSPARSER
#define CLASSPARSER
#include "classifierParser.h"
#include "../uml/class.h"
using namespace std;

class ClassParser : public ClassifierParser {
    protected:
        Element* createElement();
        bool parseTag(InputParser* input, Element * el);

    public:
        ClassParser() {
            this->keyword = "class";
        }
};

#endif