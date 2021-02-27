#include "yuml-parsers/activityEdgeParser.h"

bool ActivityEdgeParser::parseFeatures(YAML::Node node, Element* el) {

    if (node["source"]) {
        string parsedId = node["source"].as<string>();

        // assumption is that edges always defined after nodes
        if (UML::isValidUUID4(parsedId)) {
            boost::uuids::uuid sourceId = boost::lexical_cast<boost::uuids::uuid>(parsedId);

            ActivityNode* source = dynamic_cast<ActivityNode*>((*elements)[sourceId]);

            dynamic_cast<ActivityEdge*>(el)->setSource(source);
        } else {
            // TODO node definitions in edges?
        }
    }

    if (node["target"]) {
        string parsedId = node["target"].as<string>();

        if (UML::isValidUUID4(parsedId)) {
            boost::uuids::uuid targetId = boost::lexical_cast<boost::uuids::uuid>(parsedId);

            ActivityNode* target = dynamic_cast<ActivityNode*>((*elements)[targetId]);

            dynamic_cast<ActivityEdge*>(el)->setTarget(target);
        } else {
            // TODO node definitions in edges?
        }
    }
    
    if (node["guard"]) {
        if(isValidUUID4(node["guard"].as<string>())) {
            // This means it is an instance
            boost::uuids::uuid guardID = boost::lexical_cast<boost::uuids::uuid>(node["guard"].as<string>());

            parseNowOrLater(guardID, el->uuid, &ActivityEdgeParser::setGuardLater);
        } else {
            // Literal parsing has relied on finding info from rest of model so i see two solutions her
            // 1)  get info now or later from edge, guards are associated with decision nodes and object flows usually
            //     to get type of the guard so we can parse this properly
            // 2)  another way is we could get it from the format of the entry, this is less definite though
            //     would need some functions to determing type from string
            cerr << "[ERROR] Literal Guards not implemented yet!" << endl;
        }
    }

    return NamedElementParser::parseFeatures(node, el);
}

bool ActivityEdgeParser::emit(YAML::Emitter& emitter, Element* el) {

    bool ret = NamedElementParser::emit(emitter, el);

    if (dynamic_cast<ActivityEdge*>(el)->getSource() != NULL) {
        emitter << YAML::Key << "source";
        emitter << YAML::Value << boost::lexical_cast<string>(dynamic_cast<ActivityEdge*>(el)->getSource()->uuid);
    }

    if (dynamic_cast<ActivityEdge*>(el)->getTarget() != NULL) {
        emitter << YAML::Key << "target";
        emitter << YAML::Value << boost::lexical_cast<string>(dynamic_cast<ActivityEdge*>(el)->getTarget()->uuid);
    }

    return ret;
}