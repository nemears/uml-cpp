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
                if (node["inputs"][i].IsMap()) {
                    if (node["inputs"][i]["inputPin"]) {
                        InputPinParser ip(elements, postProcessFlag);
                        Element* parsedEl = ip.TypedElementParser::parseElement(node["inputs"][i]["inputPin"]);
                        dynamic_cast<Action*>(el)->inputs.push_back(dynamic_cast<InputPin*>(parsedEl));
                    }
                } else if (isValidUUID4(node["inputs"][i].as<string>())) {
                    boost::uuids::uuid inputId = boost::lexical_cast<boost::uuids::uuid>(node["inputs"][i].as<string>());

                    // check if null
                    // if null we make a flag for backwards parsing
                    if((*elements)[inputId] == 0) {

                        // check if struct created
                        if ((*postProcessFlag)[inputId] == 0) {
                            list<boost::uuids::uuid>* eList = new list<boost::uuids::uuid>;
                            list<void(*)(Element*, Element*)>* fList = new list<void(*)(Element*, Element*)>;;
                            PostParser* postParser  =  new PostParser{*eList, *fList};
                            (*postProcessFlag)[inputId] = postParser;
                        } 

                        // add flag with function pointer
                        (*postProcessFlag)[inputId]->otherEls.push_back(el->uuid);
                        (*postProcessFlag)[inputId]->applyOnEl.push_back(&ActionParser::addInputPinLater);
                    } else {
                        dynamic_cast<Action*>(el)->inputs.push_back(dynamic_cast<InputPin*>((*elements)[inputId]));
                    }
                } else {
                    YAML::Emitter errEmit;
                    errEmit << node["inputs"];
                    throw ElementParser::InvalidIdentifierException(node["inputs"][i].Mark().line, errEmit.c_str());
                }
            }
        } else {
            throw ElementParser::InvalidNodeTypeException(node["inputs"].Mark().line, "sequence");
        }
    }

    if (node["outputs"]) {
        if (node["outputs"].IsSequence()) {
            for (std::size_t i=0; i<node["outputs"].size(); i++) {
                if (node["outputs"][i].IsMap()) {
                    if (node["outputs"][i]["outputPin"]) {
                        OutputPinParser op(elements, postProcessFlag);
                        Element* parsedEl = op.TypedElementParser::parseElement(node["outputs"][i]["outputPin"]);
                        dynamic_cast<Action*>(el)->outputs.push_back(dynamic_cast<OutputPin*>(parsedEl));
                    } 
                } else if (isValidUUID4(node["outputs"][i].as<string>())) {
                    boost::uuids::uuid outputId = boost::lexical_cast<boost::uuids::uuid>(node["outputs"][i].as<string>());

                    // check if null
                    // if null we make a flag for backwards parsing
                    if((*elements)[outputId] == 0) {

                        // check if struct created
                        if ((*postProcessFlag)[outputId] == 0) {
                            list<boost::uuids::uuid>* eList = new list<boost::uuids::uuid>;
                            list<void(*)(Element*, Element*)>* fList = new list<void(*)(Element*, Element*)>;;
                            PostParser* postParser  =  new PostParser{*eList, *fList};
                            (*postProcessFlag)[outputId] = postParser;
                        } 

                        // add flag with function pointer
                        (*postProcessFlag)[outputId]->otherEls.push_back(el->uuid);
                        (*postProcessFlag)[outputId]->applyOnEl.push_back(&ActionParser::addOutputPinLater);
                    } else {
                        dynamic_cast<Action*>(el)->inputs.push_back(dynamic_cast<InputPin*>((*elements)[outputId]));
                    }
                } else {
                    YAML::Emitter errEmit;
                    errEmit << node["outputs"];
                    throw ElementParser::InvalidIdentifierException(node["outputs"][i].Mark().line, errEmit.c_str());
                }
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