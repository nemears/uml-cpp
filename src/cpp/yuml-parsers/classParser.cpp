#include "yuml-parsers/classParser.h"
#include "yuml-parsers/operationParser.h"

using namespace UML;

Element* ClassParser::createElement() {
    theEl = new Class;
    return theEl;
}

bool ClassParser::parseFeatures(YAML::Node node, Element* el) {

    if (node["operations"]) {
        if (node["operations"].IsSequence()) {
            for (std::size_t i=0; i<node["operations"].size(); i++) {
                OperationParser operationParser(elements, postProcessFlag);
                Element* parsedEl = operationParser.parseElement(node["operations"][i]["operation"]);
                dynamic_cast<Class*>(el)->getOperations().add(*dynamic_cast<Operation*>(parsedEl));
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

    if (!dynamic_cast<Class*>(el)->getOperations().empty()) {
        emitter << YAML::Key << "operations";
        emitter << YAML::BeginSeq;
        for (auto const& operation: dynamic_cast<Class*>(el)->getOperations().iterator()) {
            OperationParser op(elements, postProcessFlag);
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

ClassParser ClassParser::createNewParser() {
    return ClassParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}