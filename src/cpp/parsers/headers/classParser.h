#ifndef CLASSPARSER
#define CLASSPARSER
#include "classifierParser.h"
#include "../../uml/headers/class.h"
using namespace std;

class ClassParser : public ClassifierParser {
    protected:
        Element* createElement();
        bool parseTag(InputParser* input, Element * el);

    public:
        ClassParser(map<boost::uuids::uuid, Element*>* elements) : ClassifierParser(elements){
            this->keyword = "class";
            // this->elements = elements;
        }
};

#endif