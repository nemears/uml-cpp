#ifndef OBJECTFLOWPARSER_H
#define OBJECTFLOWPARSER_H

#include "activityEdgeParser.h"
#include "uml/objectFlow.h"

class ObjectFlowParser : public ActivityEdgeParser {
    public:
        Element* createElement() override;
        ObjectFlowParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            ActivityEdgeParser(elements, postParsers) {
            this->keyword = "objectFlow";
        }
        static ObjectFlowParser createNewParser();
};

#endif