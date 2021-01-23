#include "headers/operationParser.h"

Element* OperationParser::createElement() {
    return new Operation;
}

bool OperationParser::parseFeatures(YAML::Node node, Element* el) {

    return NamedElementParser::parseFeatures(node, el);
}