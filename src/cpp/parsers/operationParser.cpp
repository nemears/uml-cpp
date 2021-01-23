#include "headers/operationParser.h"

bool OperationParser::parseFeatures(YAML::Node node, Element* el) {

    return NamedElementParser::parseFeatures(node, el);
}