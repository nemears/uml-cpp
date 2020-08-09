#ifndef CLASSPARSER
#define CLASSPARSER
#include "namedElementParser.h"
#include "../uml/class.h"
using namespace std;

class ClassParser : public NamedElementParser {
    protected:
        Element* createElement();
        bool parseTag(InputParser* input, Element * el);

    public:
        ClassParser() {
            this->keyword = "class";
        }
};

#endif