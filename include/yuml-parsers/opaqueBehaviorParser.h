#ifndef OPAQUEBEHAVIORPARSER
#define OPAQUEBEHAVIORPARSER

#include "behaviorParser.h"
#include "uml/opaqueBehavior.h"

using namespace UML;

class OpaqueBehaviorParser : public BehaviorParser {

    protected:
        Element* createElement();
        bool parseFeatures(YAML::Node node, Element* el);

    public:
        OpaqueBehaviorParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            BehaviorParser(elements, postParsers){
            this->keyword = "opaqueBehavior";
        }
        bool emit(YAML::Emitter& emitter, Element* el);
        static OpaqueBehaviorParser createNewParser();
};

#endif