#include "yuml-parsers/directedRelationshipParser.h"

bool DirectedRelationshipParser::parseFeatures(YAML::Node node, Element* el) {
    bool ret = RelationshipParser::parseFeatures(node, el);

    if (node["sources"]) {
        if (node["sources"].IsSequence()) {
            for (std::size_t i=0; i<node["sources"].size(); i++) {
                if (node["sources"][i].IsScalar()) {
                    if (isValidUUID4(node["sources"][i].as<string>())) {
                        parseNowOrLater(boost::lexical_cast<boost::uuids::uuid>(node["sources"][i].as<string>()),
                        el->getID(),
                        node["sources"][i],
                        &DirectedRelationshipParser::addSourceLater);
                    }
                } else if (node["sources"][i].IsMap()) {
                    // Error
                    ret = false;
                }
            }
        } else {
            // Error
            ret = false;
        }
    }

    if (node["targets"]) {
        if (node["targets"].IsSequence()) {
            for (std::size_t i=0; i<node["targets"].size(); i++) {
                if (node["targets"][i].IsScalar()) {
                    if (isValidUUID4(node["targets"][i].as<string>())) {
                        parseNowOrLater(boost::lexical_cast<boost::uuids::uuid>(node["targets"][i].as<string>()),
                        el->getID(),
                        node["targets"][i],
                        &DirectedRelationshipParser::addTargetLater);
                    }
                } else if (node["targets"][i].IsMap()) {
                    // Error
                    ret = false;
                }
            }
        } else {
            // Error
            ret = false;
        }
    }

    return ret;
}

void DirectedRelationshipParser::addSourceLater(YAML::Node node, Element* relationship, Element* source) {
    dynamic_cast<DirectedRelationship*>(relationship)->getSources().add(*source);
}

void DirectedRelationshipParser::addTargetLater(YAML::Node node, Element* relationship, Element* target) {
    dynamic_cast<DirectedRelationship*>(relationship)->getTargets().add(*target);
}

bool DirectedRelationshipParser::emit(YAML::Emitter& emitter, Element* el) {
    bool ret = RelationshipParser::emit(emitter, el);

    if (!dynamic_cast<DirectedRelationship*>(el)->getTargets().empty()) {
        emitter << YAML::Key << "targets";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& target : dynamic_cast<DirectedRelationship*>(el)->getTargets()) {
            emitter << YAML::Value << target->getIDstring();
        }
    }

    if (!dynamic_cast<DirectedRelationship*>(el)->getSources().empty()) {
        emitter << YAML::Key << "sources";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& source : dynamic_cast<DirectedRelationship*>(el)->getSources()) {
            emitter << YAML::Value << source->getIDstring();
        }
    }

    return ret;
}