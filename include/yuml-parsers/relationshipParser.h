#ifndef RELATIONSHIP_PARSER_H
#define RELATIONSHIP_PARSER_H

#include "elementParser.h"
#include "uml/relationship.h"

class RelationshipParser : public ElementParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el) override;
    public:
        RelationshipParser(map<boost::uuids::uuid, UML::Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            ElementParser(elements, postParsers)
            {};
        static void addRelatedElementLater(YAML::Node node, Element* relationship, Element* relatedElement);
        bool emit(YAML::Emitter& emitter, Element* el) override;
};

#endif