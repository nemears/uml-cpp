#pragma once

#include "yaml-cpp/yaml.h"
#include "openUmlSerializationPolicy.h"
#include "uml/uml-stable.h"

namespace UML {
    // general functions for parsing
    ElementPtr parseNode(YAML::Node node, ParserData& data);
    std::string getLineNumber(YAML::Node node);

    // parsing features of element types
    void parseDataTypeFeatures(YAML::Node node, DataType& dataType, ParserData& data);
    void parseElementFeatures(YAML::Node node, Element& el, ParserData& data);
    void parseInstanceSpecificationFeatures(YAML::Node node, InstanceSpecification& inst, ParserData& data);
    void parseNamedElementFeatures(YAML::Node node, NamedElement& el, ParserData& data);
    void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data);

    // parsing scopes of element types
    bool parseElementScope(YAML::Node node, Element& el, ParserData& data);
    bool parseEnumerationLiteralScope(YAML::Node node, EnumerationLiteral& literal, ParserData& data);
    bool parsePackageableElementScope(YAML::Node node, PackageableElement& el, ParserData& data);
    // TODO

    // parsing elements
    DataTypePtr createAndParseDataType(YAML::Node node, ParserData& data);
    EnumerationLiteralPtr createAndParseEnumerationLiteral(YAML::Node node, ParserData& data);
    InstanceSpecificationPtr createAndParseInstanceSpecification(YAML::Node node, ParserData& data);
    PackagePtr createAndParsePackage(YAML::Node node, ParserData& data);

    // parsing sets
    InstanceSpecificationPtr determineAndParseInstanceSpecification(YAML::Node node, ParserData& data);
    PackageableElementPtr determineAndParsePackageableElement(YAML::Node node, ParserData& data);
}