#include "uml/parsers/namedElementParser.h"
#include "uml/parsers/elementParser.h"

using namespace UML;
using namespace Parsers;

void UML::Parsers::parseNamedElement(YAML::Node node, NamedElement& el) {

    parseElement(node, el);

    if (node["name"]) {
        if (node["name"].IsScalar()) {
            el.setName(node["name"].as<string>());
        } else {
            // error
        }
    }
}