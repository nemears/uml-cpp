#ifndef BEHAVIORPARSER_H
#define BEHAVIORPARSER_H

#include "classParser.h"
#include "parameterParser.h"
#include "uml/behavior.h"

class BehaviorParser : public ClassParser {
    protected:
        bool parseFeatures(YAML::Node node, Element* el) override;
    public:
        BehaviorParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            ClassParser(elements, postParsers) {};
        bool emit(YAML::Emitter& emitter, Element* el) override;
        static void addParameterLater(Element* behavior, Element* param) {
            dynamic_cast<Behavior*>(behavior)->parameters.push_back(dynamic_cast<Parameter*>(param));
        };
};

#endif