#include "yuml-parsers/activityNodeParser.h"

bool ActivityNodeParser::parseFeatures(YAML::Node node, Element* el) {
    
    bool ret = NamedElementParser::parseFeatures(node, el);

    if (!parseActivityNodeFeatures(node, el)) {
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

                    parseNowOrLater(incomingId, el->uuid, &ActivityNodeParser::addIncomingEdgeLater);
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

                    parseNowOrLater(outgoingId, el->uuid, &ActivityNodeParser::addOutgoingEdgeLater);
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

    bool ret = NamedElementParser::emit(emitter, el);

    if(!emitActivityNode(emitter, el)) {
        ret = false;
    }

    return ret;
}

bool ActivityNodeParser::emitActivityNode(YAML::Emitter& emitter, Element* el) {
    if (!dynamic_cast<ActivityNode*>(el)->incoming.empty()) {
        emitter << YAML::Key << "incoming";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& edge : dynamic_cast<ActivityNode*>(el)->incoming) {
            emitter << YAML::Value << edge->getIDstring();
        }
        emitter << YAML::EndSeq;
    }

    if (!dynamic_cast<ActivityNode*>(el)->outgoing.empty()) {
        emitter << YAML::Key << "outgoing";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& edge : dynamic_cast<ActivityNode*>(el)->outgoing) {
            emitter << YAML::Value << edge->getIDstring();
        }
        emitter << YAML::EndSeq;
    }

    return true;
}