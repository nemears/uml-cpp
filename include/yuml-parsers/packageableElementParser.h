#ifndef PACKAGEABLE_ELEMENT_PARSER_H
#define PACKAGEABLE_ELEMENT_PARSER_H

#include "namedElementParser.h"
#include "uml/packageableElement.h"

class PackageableElementParser : virtual public NamedElementParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el) override;
        bool parsePackageableElementFeatures(YAML::Node node, Element* el);
    public:
        PackageableElementParser(map<boost::uuids::uuid, UML::Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            NamedElementParser(elements, postParsers){};
        bool emit(YAML::Emitter& emitter, Element* el) override;
};

#endif