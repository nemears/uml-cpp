#include "headers/typedElementParser.h"

bool TypedElementParser::parseFeatures(YAML::Node node, Element* el) {
    if (node["type"]) {
        string parsedId = node["type"].as<string>();
        boost::uuids::uuid typeId = boost::lexical_cast<boost::uuids::uuid>(parsedId);

        Type* type = (Type*)(*elements)[typeId];

        ((TypedElement*)el)->setType(type);
    }

    return NamedElementParser::parseFeatures(node, el);
}