#ifndef OPERATIONPARSER
#define OPERATIONPARSER

#include "namedElementParser.h"
#include "parameterParser.h"
#include "opaqueBehaviorParser.h"
#include "uml/operation.h"
#include "uml/sequence.h"
#include "activityParser.h"

using namespace UML;

class OperationParser : public NamedElementParser {
    protected:
        UML::Element* createElement();
        bool parseFeatures(YAML::Node node, UML::Element* el);

    public:
        OperationParser(map<boost::uuids::uuid, UML::Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            NamedElementParser(elements, postParsers){
            this->keyword = "operation";
        };
        bool emit(YAML::Emitter& emitter, Element* el);
        static OperationParser createNewParser();
        static void addMethodLater(YAML::Node node, Element* operation, Element* method) {
            dynamic_cast<Operation*>(operation)->getMethods().add(*dynamic_cast<Behavior*>(method));
        };
};

#endif