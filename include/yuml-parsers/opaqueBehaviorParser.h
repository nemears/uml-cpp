#ifndef OPAQUEBEHAVIORPARSER
#define OPAQUEBEHAVIORPARSER

#include "classParser.h"
#include "uml/opaqueBehavior.h"

using namespace UML;

class OpaqueBehaviorParser : public ClassParser {

    protected:
        Element* createElement();
        bool parseFeatures(YAML::Node node, Element* el);

    public:
        OpaqueBehaviorParser(map<boost::uuids::uuid, Element*>* elements) : ClassParser(elements){
            this->keyword = "opaqueBehavior";
        }
        bool emit(YAML::Emitter& emitter, Element* el);
};

#endif