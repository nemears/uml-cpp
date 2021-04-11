#include "uml/parsers/propertyParser.h"
#include "uml/parsers/typedElementParser.h"
using namespace UML;
using namespace Parsers;

void UML::Parsers::parseProperty(YAML::Node node, Property& prop, ParserMetaData& data) {
    parseTypedElement(node, prop, data);

    if (node["defaultValue"]) {
        // TODO
    }
}