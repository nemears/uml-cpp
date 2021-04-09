#include "uml/parsers/parser.h"

UML::Element* UML::Parsers::parse(YAML::Node node) {
    if (node["package"]) {
        Package* pckg = new Package;
        UML::Parsers::parsePackage(node["package"], *pckg);
        return pckg;
    }

    return 0;
}