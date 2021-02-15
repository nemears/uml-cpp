#include "yuml-parsers/actionParser.h"

bool ActionParser::parseFeatures(YAML::Node node, Element* el) {

    if (node["inputs"]) {
        if (node["inputs"].IsSequence()) {
            for (std::size_t i=0; i<node["inputs"].size(); i++) {
                // TODO inputPin parsers
            }
        } else {
            throw ElementParser::InvalidNodeTypeException(node["inputs"].Mark().line, "sequence");
        }
    }

    if (node["outputs"]) {
        if (node["outputs"].IsSequence()) {
            for (std::size_t i=0; i<node["inputs"].size(); i++) {
                // TODO outputPin parsers
            }
        } else {
            throw ElementParser::InvalidNodeTypeException(node["outputs"].Mark().line, "sequence");
        }
    }

    return ActivityNodeParser::parseFeatures(node, el);
}

bool ActionParser::emit(YAML::Emitter& emitter, Element* el) {

    if (el->getElementType() == ElementType::ACTION) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "action";
        emitter << YAML::BeginMap;
    }

    if (!dynamic_cast<Action*>(el)->inputs.empty()) {
        for (auto const& input : dynamic_cast<Action*>(el)->inputs) {
            // TODO inputPin parser
        }
    }

    if (!dynamic_cast<Action*>(el)->outputs.empty()) {
        for (auto const& input : dynamic_cast<Action*>(el)->outputs) {
            // TODO inputPin parser
        }
    }

    bool ret = ActivityNodeParser::emit(emitter, el);

    if (el->getElementType() == ElementType::ACTION) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}