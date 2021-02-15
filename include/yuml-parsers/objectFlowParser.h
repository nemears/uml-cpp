#ifndef OBJECTFLOWPARSER_H
#define OBJECTFLOWPARSER_H

#include "activityEdgeParser.h"
#include "uml/objectFlow.h"

class ObjectFlowParser : public ActivityEdgeParser {
    public:
        Element* createElement() override;
        ObjectFlowParser(map<boost::uuids::uuid, Element*>* elements) : ActivityEdgeParser(elements) {
            this->keyword = "objectFlow";
        }
};

#endif