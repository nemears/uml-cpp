#include "uml/parsers/parser.h"
#include "uml/parsers/packageParser.h"
#include "uml/parsers/classParser.h"

using namespace UML;
using namespace Parsers;

UML::Element* UML::Parsers::parse(YAML::Node node) {
    if (node["class"]) {
        Class* clazz = new Class;
        parseClass(node["class"], *clazz);
        return clazz;
    }

    if (node["package"]) {
        Package* pckg = new Package;
        UML::Parsers::parsePackage(node["package"], *pckg);
        return pckg;
    }

    return 0;
}