#pragma once

#include "yaml-cpp/yaml.h"
#include "openUmlSerializationPolicy.h"
#include "uml/uml-stable.h"

namespace UML {
    // general functions for parsing
    ElementPtr parseNode(YAML::Node node, ParserData& data);
    std::string getLineNumber(YAML::Node node);

    // parsing features of element types
    void parseElementFeatures(YAML::Node node, Element& el, ParserData& data);
    void parseInstanceSpecificationFeatures(YAML::Node node, InstanceSpecification& inst, ParserData& data);
    void parseNamedElementFeatures(YAML::Node node, NamedElement& el, ParserData& data);

    // parsing scopes of element types
    bool parseElementScope(YAML::Node node, Element& el, ParserData& data);
    bool parseEnumerationLiteralScope(YAML::Node node, EnumerationLiteral& literal, ParserData& data);
    // TODO

    // parsing elements
    EnumerationLiteralPtr createAndParseEnumerationLiteral(YAML::Node node, ParserData& data);
    InstanceSpecificationPtr createAndParseInstanceSpecification(YAML::Node node, ParserData& data);

    // parsing sets
    InstanceSpecificationPtr determineAndParseInstanceSpecification(YAML::Node node, ParserData& data);
}