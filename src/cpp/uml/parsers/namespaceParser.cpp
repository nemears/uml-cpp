#include "uml/parsers/namespaceParser.h"
#include "uml/parsers/namedElementParser.h"

using namespace UML;
using namespace Parsers;

void UML::Parsers::parseNamespace(YAML::Node node, Namespace& nmspc, ParserMetaData& data) {
    parseNamedElement(node, nmspc, data);

    if(node["members"]) {
        // TODO validate
    }
}