#ifndef CONTROLFLOWPARSER_H
#define CONTROLFLOWPARSER_H

#include "activityEdgeParser.h"
#include "uml/controlFlow.h"

class ControlFlowParser : public ActivityEdgeParser {
    public:
        Element* createElement() override;
        ControlFlowParser(map<boost::uuids::uuid, Element*>* elements) : ActivityEdgeParser(elements) {
            this->keyword = "controlFlow";
        };
};

#endif