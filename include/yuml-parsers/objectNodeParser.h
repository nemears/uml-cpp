#ifndef OBJECTNODEPARSER_H
#define OBJECTNODEPARSER_H

#include "activityNodeParser.h"
#include "typedElementParser.h"

class ObjectNodeParser : public ActivityNodeParser, public TypedElementParser {
    protected:
    bool parseFeatures(YAML::Node node, Element* el) override;

    public:
    ObjectNodeParser(map<boost::uuids::uuid, Element*>* elements) : ActivityNodeParser(elements), TypedElementParser(elements) {
        this->keyword = "objectNode";
    };
    bool emit(YAML::Emitter& emitter, Element* el) override;

};

#endif