#ifndef OBJECTNODEPARSER_H
#define OBJECTNODEPARSER_H

#include "activityNodeParser.h"
#include "typedElementParser.h"
#include "uml/objectNode.h"

class ObjectNodeParser : public ActivityNodeParser, public TypedElementParser {
    protected:
        bool parseFeatures(YAML::Node node, Element* el) override;
        bool parseObjectNodeFeatures(YAML::Node node, Element* el);
        bool emitObjectNode(YAML::Emitter& emitter, Element* el);

    public:
        ObjectNodeParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            ActivityNodeParser(elements, postParsers), 
            TypedElementParser(elements, postParsers), 
            NamedElementParser(elements, postParsers) {
            this->keyword = "objectNode";
        };
        bool emit(YAML::Emitter& emitter, Element* el) override;
        Element* createElement() override;
        static ObjectNodeParser createNewParser();
};

#endif