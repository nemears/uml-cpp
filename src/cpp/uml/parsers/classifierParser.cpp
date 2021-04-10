#include "uml/parsers/classifierParser.h"
#include "uml/parsers/namespaceParser.h"

using namespace UML;
using namespace Parsers;

void UML::Parsers::parseClassifier(YAML::Node node, Classifier& clazz) {
    parseNamespace(node, clazz);

    if (node["attributes"]) {
        // TODO
    }
}