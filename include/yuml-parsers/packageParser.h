#ifndef PACKAGE_PARSER_H
#define PACKAGE_PARSER_H

#include "namespaceParser.h"
#include "packageableElementParser.h"

class PackageParser : public NamespaceParser, public PackageableElementParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el) override;
        Element* createElement() override;
    public:
        PackageParser(map<boost::uuids::uuid, UML::Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            NamespaceParser(elements, postParsers),
            PackageableElementParser(elements, postParsers),
            NamedElementParser(elements, postParsers){};
        bool emit(YAML::Emitter& emitter, Element* el) override;
};

#endif