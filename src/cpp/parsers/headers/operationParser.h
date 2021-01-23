#ifndef OPERATIONPARSER
#define OPERATIONPARSER

#include "namedElementParser.h"
#include "parameterParser.h"
#include "opaqueBehaviorParser.h"
#include "../../uml/headers/operation.h"

using namespace UML;

class OperationParser : public NamedElementParser {
    protected:
        UML::Element* createElement();
        bool parseFeatures(YAML::Node node, UML::Element* el);

    public:
        OperationParser(map<boost::uuids::uuid, UML::Element*>* elements) : NamedElementParser(elements){
            this->keyword = "operation";
        };
};

#endif