#include "headers/typedElementParser.h"

bool TypedElementParser::parseFeatures(YAML::Node node, UML::Element* el) {
    if (node["type"]) {
        string parsedId = node["type"].as<string>();
        boost::uuids::uuid typeId = boost::lexical_cast<boost::uuids::uuid>(parsedId);

        UML::Type* type = (UML::Type*)(*elements)[typeId];

        ((UML::TypedElement*)el)->setType(type);
    }

    return NamedElementParser::parseFeatures(node, el);
}