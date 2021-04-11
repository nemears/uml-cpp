#include "uml/parsers/packageableElementParser.h"

using namespace UML;

void Parsers::parsePackageableElement(YAML::Node node, PackageableElement& el, ParserMetaData& data) {
    if (node["owningPackage"]) {
        // TODO validate
    }
}