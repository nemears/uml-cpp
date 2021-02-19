#include "yuml-parsers/actionParser.h"
#include "uml/activity.h"

Element* ActionParser::createElement() {
    theEl = new Action;
    return theEl;
}

bool ActionParser::parseFeatures(YAML::Node node, Element* el) {

    bool ret = ActivityNodeParser::parseFeatures(node, el);

    if (node["inputs"]) {
        if (node["inputs"].IsSequence()) {
            for (std::size_t i=0; i<node["inputs"].size(); i++) {
                InputPinParser ip(elements, postProcessFlag);
                Element* parsedEl = ip.TypedElementParser::parseElement(node["inputs"][i]["inputPin"]); // this should also be able to be an id
                dynamic_cast<Action*>(el)->inputs.push_back(dynamic_cast<InputPin*>(parsedEl));
            }
        } else {
            throw ElementParser::InvalidNodeTypeException(node["inputs"].Mark().line, "sequence");
        }
    }

    if (node["outputs"]) {
        if (node["outputs"].IsSequence()) {
            for (std::size_t i=0; i<node["inputs"].size(); i++) {
                OutputPinParser op(elements, postProcessFlag);
                Element* parsedEl = op.TypedElementParser::parseElement(node["outputs"][i]["outputPin"]);
                dynamic_cast<Action*>(el)->outputs.push_back(dynamic_cast<OutputPin*>(parsedEl));
            }
        } else {
            throw ElementParser::InvalidNodeTypeException(node["outputs"].Mark().line, "sequence");
        }
    }

    return ret;
}

bool ActionParser::emit(YAML::Emitter& emitter, Element* el) {

    if (el->getElementType() == ElementType::ACTION) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "action";
        emitter << YAML::BeginMap;
    }

    if (!dynamic_cast<Action*>(el)->inputs.empty()) {
        for (auto const& input : dynamic_cast<Action*>(el)->inputs) {
            InputPinParser ip(elements, postProcessFlag);
            ip.emit(emitter, input);
        }
    }

    if (!dynamic_cast<Action*>(el)->outputs.empty()) {
        for (auto const& output : dynamic_cast<Action*>(el)->outputs) {
            OutputPinParser op(elements, postProcessFlag);
            op.emit(emitter, output);
        }
    }

    bool ret = ActivityNodeParser::emit(emitter, el);

    if (el->getElementType() == ElementType::ACTION) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

ActionParser ActionParser::createNewParser() {
    return ActionParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}