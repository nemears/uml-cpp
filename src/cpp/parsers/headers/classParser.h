#ifndef CLASSPARSER
#define CLASSPARSER
#include "classifierParser.h"
#include "operationParser.h"
#include "../../uml/headers/class.h"
using namespace std;

class ClassParser : public ClassifierParser {
    protected:
        UML::Element* createElement();
        bool parseFeatures(YAML::Node node, UML::Element* el);

    public:
        ClassParser(map<boost::uuids::uuid, UML::Element*>* elements) : ClassifierParser(elements){
            this->keyword = "class";
            // this->elements = elements;
        }
};

#endif