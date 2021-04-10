#include "uml/parsers/classParser.h"
#include "uml/parsers/classifierParser.h"

using namespace UML;
using namespace Parsers;

void UML::Parsers::parseClass(YAML::Node node, Class& clazz) {
    parseClassifier(node, clazz);

    if (node["operations"]) {
        // TODO
    }
}