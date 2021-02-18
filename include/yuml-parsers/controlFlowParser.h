#ifndef CONTROLFLOWPARSER_H
#define CONTROLFLOWPARSER_H

#include "activityEdgeParser.h"
#include "uml/controlFlow.h"

class ControlFlowParser : public ActivityEdgeParser {
    public:
        Element* createElement() override;
        ControlFlowParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            ActivityEdgeParser(elements, postParsers) {
            this->keyword = "controlFlow";
        };
        bool emit(YAML::Emitter& emitter, Element* el) override;
        static ControlFlowParser createNewParser();
};

#endif