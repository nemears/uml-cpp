#include "yuml-parsers/opaqueBehaviorParser.h"

Element* OpaqueBehaviorParser::createElement() {
    return new OpaqueBehavior;
}

bool OpaqueBehaviorParser::parseFeatures(YAML::Node node, Element* el) {

    // just 1 for now TODO
    if (node["body"]) {
        ((OpaqueBehavior*) el)->bodies.push_back(node["body"].as<string>());
    }

    return ClassParser::parseFeatures(node, el);
}

bool OpaqueBehaviorParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::OPAQUE_BEHAVIOR) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "opaqueBehavior";
        emitter << YAML:: BeginMap;
    }

    bool ret = ClassParser::emit(emitter, el);

    // TODO multiples
    if (!((OpaqueBehavior*)el)->bodies.empty()) {
        emitter << YAML::Key << "body";
        emitter << YAML::Value << ((OpaqueBehavior*) el)->bodies.front();
    }

    if (el->getElementType() ==  ElementType::OPAQUE_BEHAVIOR) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}