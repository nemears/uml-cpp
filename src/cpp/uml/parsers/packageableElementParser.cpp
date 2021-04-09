#include "uml/parsers/packageableElementParser.h"

using namespace UML;

void Parsers::parsePackageableElement(YAML::Node node, PackageableElement& el) {
    if (node["owningPackage"]) {
        // TODO validate
    }
}