#include "yuml-parsers/activityParser.h"

using namespace UML;

Element* ActivityParser::createElement() {
    theEl = new Activity;
    return theEl;
}

bool ActivityParser::parseFeatures(YAML::Node node, Element* el) {

    // parse nodes
    if (node["nodes"]) {
        if (node["nodes"].IsSequence()) {
            for (std::size_t i=0; i<node["nodes"].size(); i++) {
                if (node["nodes"][i]["action"]) {
                    ActionParser actionParser(elements);
                    Element* parsedEl = actionParser.parseElement(node["nodes"][i]["action"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<Action*>(parsedEl));
                } else if (node["nodes"][i]["inputPin"]) {
                    InputPinParser inputPinParser(elements);
                    Element* parsedEl = inputPinParser.TypedElementParser::parseElement(node["nodes"][i]["inputPin"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<InputPin*>(parsedEl));
                } else if (node["nodes"][i]["objectNode"]) {
                    ObjectNodeParser objectNodeParser(elements);
                    Element* parsedEl = objectNodeParser.parseElement(node["nodes"][i]["objectNode"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ObjectNode*>(parsedEl));
                } else if (node["nodes"][i]["outputPin"]) {
                    OutputPinParser outputPinParser(elements);
                    Element* parsedEl = outputPinParser.TypedElementParser::parseElement(node["nodes"][i]["outputPin"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<OutputPin*>(parsedEl));
                } else {
                    // TODO error
                }
            }
        } else {
            throw ElementParser::InvalidNodeTypeException(node["nodes"].Mark().line, "sequence");
        }
    }

    // parse edges
    if (node["edges"]) {
        if (node["edges"].IsSequence()) {
            for (std::size_t i=0; i<node["edges"].size(); i++) {
                // TODO Edge parsers
            }
        } else {
            throw ElementParser::InvalidNodeTypeException(node["edges"].Mark().line, "sequence");
        }
    }

    return ClassParser::parseFeatures(node, el);
}

bool ActivityParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::ACTIVITY) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "activity";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = ClassParser::emit(emitter, el);

    if (!dynamic_cast<Activity*>(el)->nodes.empty()) {
        for(auto const& node : dynamic_cast<Activity*>(el)->nodes) {
            // TODO node parsers
        }
    }

    if (!dynamic_cast<Activity*>(el)->edges.empty()) {
        for (auto const& edge: dynamic_cast<Activity*>(el)->edges) {
            // TODO edge parsers
        }
    }

    if (el->getElementType() == ElementType::ACTIVITY) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}