#ifndef ACTIVITYEDGEPARSER_H
#define ACTIVITYEDGEPARSER_H

#include "namedElementParser.h"
#include "uml/activityEdge.h"
#include "uml/activityNode.h"

class ActivityEdgeParser : public NamedElementParser {
    protected:
        bool parseFeatures(YAML::Node node, Element* el) override;
    public:
        bool emit(YAML::Emitter& emitter, Element* el) override;
        ActivityEdgeParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            NamedElementParser(elements, postParsers) {};
};

#endif