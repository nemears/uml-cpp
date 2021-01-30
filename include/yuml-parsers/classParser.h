#ifndef CLASSPARSER
#define CLASSPARSER
#include "classifierParser.h"
#include "uml/class.h"
using namespace std;

/**
 * Forward declaration of OperationParser to have it compile
 **/
class OperationParser;

class ClassParser : public ClassifierParser {
    protected:
        UML::Element* createElement();
        bool parseFeatures(YAML::Node node, UML::Element* el);

    public:
        ClassParser(map<boost::uuids::uuid, UML::Element*>* elements) : ClassifierParser(elements){
            this->keyword = "class";
            // this->elements = elements;
        }

        bool emit(YAML::Emitter& emitter, Element* el);
};

#endif