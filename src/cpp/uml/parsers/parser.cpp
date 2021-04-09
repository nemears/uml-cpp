#include "uml/parsers/parser.h"

using namespace UML;
using namespace Parsers;

UML::Element* UML::Parsers::parse(YAML::Node node) {
    if (node["package"]) {
        Package* pckg = new Package;
        UML::Parsers::parsePackage(node["package"], *pckg);
        return pckg;
    }

    return 0;
}