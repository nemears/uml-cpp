#include "uml/parsers/parser.h"
#include "uml/parsers/packageParser.h"
#include "uml/parsers/classParser.h"

using namespace UML;
using namespace Parsers;

UML::Element* UML::Parsers::parse(YAML::Node node) {
    if (node["class"]) {
        Class* clazz = new Class;
        ParserMetaData data;
        parseClass(node["class"], *clazz, data);
        return clazz;
    }

    if (node["package"]) {
        Package* pckg = new Package;
        ParserMetaData data;
        UML::Parsers::parsePackage(node["package"], *pckg, data);
        return pckg;
    }

    return 0;
}