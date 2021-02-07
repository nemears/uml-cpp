#include "yuml-parsers/classParser.h"
#include "yuml-parsers/operationParser.h"

using namespace UML;

Element* ClassParser::createElement() {
    return new Class;
}

bool ClassParser::parseFeatures(YAML::Node node, Element* el) {

    if (node["operations"]) {
        if (node["operations"].IsSequence()) {
            for (std::size_t i=0; i<node["operations"].size(); i++) {
                OperationParser operationParser(elements);
                Element* parsedEl = operationParser.parseElement(node["operations"][i]["operation"]);
                ((Class*) el)->operations.push_back((Operation*) parsedEl);
            }
        } else {
            throw ElementParser::InvalidNodeTypeException(node["operations"].Mark().line, "sequence");
        }
    }

    return ClassifierParser::parseFeatures(node, el);
}

bool ClassParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::CLASS) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "class";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = ClassifierParser::emit(emitter, el);

    if (!((Class*)el)->operations.empty()) {
        emitter << YAML::Key << "operations";
        emitter << YAML::BeginSeq;
        for (auto const& operation: ((Class*)el)->operations) {
            OperationParser op(elements);
            if (!op.emit(emitter, operation)) {
                return false;
            }
        }
        emitter << YAML::EndSeq;
    }

    if (el->getElementType() == ElementType::CLASS) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}