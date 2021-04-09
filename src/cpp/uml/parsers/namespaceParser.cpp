#include "uml/parsers/namespaceParser.h"
#include "uml/parsers/namedElementParser.h"

using namespace UML;
using namespace Parsers;

void UML::Parsers::parseNamespace(YAML::Node node, Namespace& nmspc) {
    parseNamedElement(node, nmspc);

    if(node["members"]) {
        // TODO validate
    }
}