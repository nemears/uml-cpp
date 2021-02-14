#include "yuml-parsers/activityNodeParser.h"

bool ActivityNodeParser::parseFeatures(YAML::Node node, Element* el) {
    
    bool ret = parseActivityNodeFeatures(node, el);
    
    if (!NamedElementParser::parseFeatures(node, el)) {
        ret = false;
    }

    return ret;
}

bool ActivityNodeParser::parseActivityNodeFeatures(YAML::Node node, Element* el) {
    if (node["incoming"]) {
        if (node["incoming"].IsSequence()) {
            for (std::size_t i=0; i<node["incoming"].size(); i++) {
                // TODO Edge parsers
            }
        } else {
            throw ElementParser::InvalidNodeTypeException(node["incoming"].Mark().line, "sequence");
        }
    }

    if (node["outgoing"]) {
        if (node["outgoing"].IsSequence()) {
            for (std::size_t i=0; i<node["outgoing"].size(); i++) {
                // TODO Edge parsers
            }
        } else {
            throw ElementParser::InvalidNodeTypeException(node["outgoing"].Mark().line, "sequence");
        }
    }

    return true;
}

bool ActivityNodeParser::emit(YAML::Emitter& emitter, Element* el) {

    if (!dynamic_cast<ActivityNode*>(el)->incoming.empty()) {
        for (auto const& edge : dynamic_cast<ActivityNode*>(el)->incoming) {
            // TODO Edge parsers
        }
    }

    if (!dynamic_cast<ActivityNode*>(el)->outgoing.empty()) {
        for (auto const& edge : dynamic_cast<ActivityNode*>(el)->outgoing) {
            // TODO Edge parsers
        }
    }

    return NamedElementParser::emit(emitter, el);
}