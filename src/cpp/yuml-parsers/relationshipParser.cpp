#include "yuml-parsers/relationshipParser.h"

bool RelationshipParser::parseFeatures(YAML::Node node, UML::Element* el) {

    bool ret = ElementParser::parseFeatures(node, el);
    
    if (node["relatedElements"]) {
        if (node["relatedElements"].IsSequence()) {
            for (std::size_t i=0; i<node["relatedElements"].size(); i++) {
                if (node["relatedElements"][i].IsScalar()) {
                    if (isValidUUID4(node["relatedElements"][i].as<string>())) {
                        parseNowOrLater(boost::lexical_cast<boost::uuids::uuid>(node["relatedElements"][i].as<string>()),
                                        el->getID(),
                                        node["relatedElements"][i],
                                        &RelationshipParser::addRelatedElementLater);
                    }
                } else if (node["relatedElements"][i].IsMap()) {
                    // error
                }
            }
        }
    }

    return ret;
}

void RelationshipParser::addRelatedElementLater(YAML::Node node, Element* relationship, Element* relatedElement) {
    dynamic_cast<Relationship*>(relationship)->getRelatedElements().add(*relatedElement);
}

bool RelationshipParser::emit(YAML::Emitter& emitter, Element* el) {
    bool ret = ElementParser::emit(emitter, el);

    if (!dynamic_cast<Relationship*>(el)->getRelatedElements().empty()) {
        emitter << YAML::Key << "relatedElements";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& relatedElement : dynamic_cast<Relationship*>(el)->getRelatedElements()) {
            emitter << YAML::Value << relatedElement->getIDstring();
        }
        emitter << YAML::EndSeq;
    }

    return ret;
}