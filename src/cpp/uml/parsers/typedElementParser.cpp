#include "uml/parsers/typedElementParser.h"
#include "uml/parsers/namedElementParser.h"

using namespace UML;
using namespace Parsers;

void UML::Parsers::parseTypedElement(YAML::Node node, TypedElement& el, ParserMetaData& data) {

    parseNamedElement(node, el, data);

    if (node["type"]) {
        // TODO
    }
}