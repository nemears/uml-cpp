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

                string parsedId = node["incoming"][i].as<string>();

                if (UML::isValidUUID4(parsedId)) {
                    boost::uuids::uuid incomingId = boost::lexical_cast<boost::uuids::uuid>(parsedId);

                    ActivityEdge* incomingEdge = dynamic_cast<ActivityEdge*>((*elements)[incomingId]);

                    dynamic_cast<ActivityNode*>(el)->incoming.push_back(incomingEdge);
                } else {
                    // TODO node definitions in nodes?
                }
            }
        } else {
            throw ElementParser::InvalidNodeTypeException(node["incoming"].Mark().line, "sequence");
        }
    }

    if (node["outgoing"]) {
        if (node["outgoing"].IsSequence()) {
            for (std::size_t i=0; i<node["outgoing"].size(); i++) {
                string parsedId = node["outgoing"][i].as<string>();

                if (UML::isValidUUID4(parsedId)) {
                    boost::uuids::uuid outgoingId = boost::lexical_cast<boost::uuids::uuid>(parsedId);

                    ActivityEdge* outgoingEdge = dynamic_cast<ActivityEdge*>((*elements)[outgoingId]);

                    dynamic_cast<ActivityNode*>(el)->outgoing.push_back(outgoingEdge);
                } else {
                    // TODO node definitions in nodes?
                }
            }
        } else {
            throw ElementParser::InvalidNodeTypeException(node["outgoing"].Mark().line, "sequence");
        }
    }

    return true;
}

bool ActivityNodeParser::emit(YAML::Emitter& emitter, Element* el) {

    bool ret = emitActivityNode(emitter, el);

    if(!NamedElementParser::emit(emitter, el)) {
        ret = false;
    }

    return ret;
}

bool ActivityNodeParser::emitActivityNode(YAML::Emitter& emitter, Element* el) {
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

    return true;
}