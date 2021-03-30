#include "yuml-parsers/generalizationParser.h"

bool GeneralizationParser::parseFeatures(YAML::Node node, Element* el) {

    bool ret = DirectedRelationshipParser::parseFeatures(node, el);

    if (node["general"]) {
        if (node["general"].IsScalar()) {
            if (isValidUUID4(node["general"].as<string>())) {
                parseNowOrLater(boost::lexical_cast<boost::uuids::uuid>(node["general"].as<string>()),
                                el->getID(),
                                node["general"],
                                &GeneralizationParser::setGeneralLater);
            }
        } else {
            // Error
            ret = false;
        }
    }

    if (node["specific"]) {
        if (node["specific"].IsScalar()) {
            if (isValidUUID4(node["specific"].as<string>())) {
                parseNowOrLater(boost::lexical_cast<boost::uuids::uuid>(node["specific"].as<string>()),
                                el->getID(),
                                node["specific"],
                                &GeneralizationParser::setSpecificLater);
            }
        } else {
            // Error
            ret = false;
        }
    }

    return ret;
}

void GeneralizationParser::setSpecificLater(YAML::Node node, Element* generalization, Element* specific) {
    dynamic_cast<Generalization*>(generalization)->setSpecific(dynamic_cast<Classifier*>(specific));
}

void GeneralizationParser::setGeneralLater(YAML::Node node, Element* generalization, Element* general) {
    Classifier* gg = dynamic_cast<Classifier*>(general);
    dynamic_cast<Generalization*>(generalization)->setGeneral(dynamic_cast<Classifier*>(general));
}

Element* GeneralizationParser::createElement() {
    theEl = new Generalization;
    return theEl;
}

bool GeneralizationParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::GENERALIZATION) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "generalization";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = DirectedRelationshipParser::emit(emitter, el);

    if (dynamic_cast<Generalization*>(el)->getGeneral()) {
        emitter << YAML::Key << "general";
        emitter << YAML::Value << dynamic_cast<Generalization*>(el)->getGeneral()->getIDstring();
    }

    if (dynamic_cast<Generalization*>(el)->getSpecific()) {
        emitter << YAML::Key << "specific";
        emitter << YAML::Value << dynamic_cast<Generalization*>(el)->getSpecific()->getIDstring();
    }

    if (el->getElementType() == ElementType::GENERALIZATION) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}