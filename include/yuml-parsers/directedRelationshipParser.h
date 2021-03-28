#ifndef DIRECTED_RELATIONSHIP_PARSER_H
#define DIRECTED_RELATIONSHIP_PARSER_H

#include "relationshipParser.h"
#include "uml/directedRelationship.h"

class DirectedRelationshipParser : public RelationshipParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el) override;
    public:
        DirectedRelationshipParser(map<boost::uuids::uuid, UML::Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            RelationshipParser(elements, postParsers)
            {};
        static void addSourceLater(YAML::Node node, Element* relationship, Element* source);
        static void addTargetLater(YAML::Node node, Element* relationship, Element* target);
};

#endif