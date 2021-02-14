#include "yuml-parsers/multiplicityElementParser.h"

using namespace UML;

bool MultiplicityElementParser::parseFeatures(YAML::Node node, Element* el) {
    if (node["lower"]) {
        dynamic_cast<MultiplicityElement*>(el)->setLower(node["lower"].as<int>());
    }

    if (node["upper"]) {
        dynamic_cast<MultiplicityElement*>(el)->setUpper(node["upper"].as<int>());
    }

    return true;
}

bool MultiplicityElementParser::emit(YAML::Emitter& emitter, Element* el) {

    if (dynamic_cast<MultiplicityElement*>(el)->multiplicitySpecified()) {
        emitter << YAML::Key << "lower";
        emitter << YAML::Value << dynamic_cast<MultiplicityElement*>(el)->getLower();
        emitter << YAML::Key << "upper";
        emitter << YAML::Value << dynamic_cast<MultiplicityElement*>(el)->getUpper();
    }

    return true;
}