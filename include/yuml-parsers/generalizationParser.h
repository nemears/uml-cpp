#ifndef GENERALIZATION_PARSER_H
#define GENERALIZATION_PARSER_H

#include "directedRelationshipParser.h"
#include "uml/generalization.h"
#include "uml/classifier.h"

class GeneralizationParser : public DirectedRelationshipParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el) override;
    public:
        GeneralizationParser(map<boost::uuids::uuid, UML::Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            DirectedRelationshipParser(elements, postParsers)
            {};
        static void setSpecificLater(YAML::Node node, Element* generalization, Element* specific);
        static void setGeneralLater(YAML::Node node, Element* generalization, Element* general);
        Element* createElement() override;
};

#endif