#include "yuml-parsers/opaqueBehaviorParser.h"

Element* OpaqueBehaviorParser::createElement() {
    return new OpaqueBehavior;
}

bool OpaqueBehaviorParser::parseFeatures(YAML::Node node, Element* el) {

    // just 1 for now TODO
    if (node["body"]) {
        dynamic_cast<OpaqueBehavior*>(el)->bodies.push_back(node["body"].as<string>());
    }

    return BehaviorParser::parseFeatures(node, el);
}

bool OpaqueBehaviorParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::OPAQUE_BEHAVIOR) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "opaqueBehavior";
        emitter << YAML:: BeginMap;
    }

    bool ret = BehaviorParser::emit(emitter, el);

    // TODO multiples
    if (!dynamic_cast<OpaqueBehavior*>(el)->bodies.empty()) {
        emitter << YAML::Key << "body";
        emitter << YAML::Value << dynamic_cast<OpaqueBehavior*>(el)->bodies.front();
    }

    if (el->getElementType() ==  ElementType::OPAQUE_BEHAVIOR) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

OpaqueBehaviorParser OpaqueBehaviorParser::createNewParser() {
    return OpaqueBehaviorParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}