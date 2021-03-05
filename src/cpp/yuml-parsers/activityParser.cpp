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
                    ActionParser actionParser(elements, postProcessFlag);
                    Element* parsedEl = actionParser.parseElement(node["nodes"][i]["action"]);

                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(parsedEl));
                    dynamic_cast<ActivityNode*>(parsedEl)->setActivity(dynamic_cast<Activity*>(el));

                    for (auto const& pin : dynamic_cast<Action*>(parsedEl)->inputs) {
                        pin->setActivity(dynamic_cast<Activity*>(el));
                        dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(pin));
                    }

                    for (auto const& pin : dynamic_cast<Action*>(parsedEl)->outputs) {
                        pin->setActivity(dynamic_cast<Activity*>(el));
                        dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(pin));
                    }
                } else if (node["nodes"][i]["callBehaviorAction"]) {
                    CallBehaviorActionParser callBehaviorActionParser(elements, postProcessFlag);
                    Element* parsedEl = callBehaviorActionParser.parseElement(node["nodes"][i]["callBehaviorAction"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(parsedEl));
                    dynamic_cast<ActivityNode*>(parsedEl)->setActivity(dynamic_cast<Activity*>(el));

                    for (auto const& pin : dynamic_cast<Action*>(parsedEl)->inputs) {
                        pin->setActivity(dynamic_cast<Activity*>(el));
                        dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(pin));
                    }

                    for (auto const& pin : dynamic_cast<Action*>(parsedEl)->outputs) {
                        pin->setActivity(dynamic_cast<Activity*>(el));
                        dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(pin));
                    }
                } else if (node["nodes"][i]["createObjectAction"]) {
                    CreateObjectActionParser createObjectActionParser(elements, postProcessFlag);
                    Element* parsedEl = createObjectActionParser.parseElement(node["nodes"][i]["createObjectAction"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(parsedEl));
                    dynamic_cast<ActivityNode*>(parsedEl)->setActivity(dynamic_cast<Activity*>(el));

                    for (auto const& pin : dynamic_cast<Action*>(parsedEl)->inputs) {
                        pin->setActivity(dynamic_cast<Activity*>(el));
                        dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(pin));
                    }

                    for (auto const& pin : dynamic_cast<Action*>(parsedEl)->outputs) {
                        pin->setActivity(dynamic_cast<Activity*>(el));
                        dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(pin));
                    }
                } else if (node["nodes"][i]["decisionNode"]) {
                    DecisionNodeParser decisionNodeParser(elements, postProcessFlag);
                    Element* parsedEl = decisionNodeParser.parseElement(node["nodes"][i]["decisionNode"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(parsedEl));
                    dynamic_cast<ActivityNode*>(parsedEl)->setActivity(dynamic_cast<Activity*>(el));
                } else if (node ["nodes"][i]["finalNode"]) {
                    FinalNodeParser finalNodeParser(elements, postProcessFlag);
                    Element* parsedEl = finalNodeParser.parseElement(node["nodes"][i]["finalNode"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(parsedEl));
                    dynamic_cast<ActivityNode*>(parsedEl)->setActivity(dynamic_cast<Activity*>(el));
                } else if (node["nodes"][i]["forkNode"]) {
                    ForkNodeParser forkNodeParser(elements, postProcessFlag);
                    Element* parsedEl = forkNodeParser.parseElement(node["nodes"][i]["forkNode"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(parsedEl));
                    dynamic_cast<ActivityNode*>(parsedEl)->setActivity(dynamic_cast<Activity*>(el));
                } else if (node["nodes"][i]["initialNode"]) {
                    InitialNodeParser initialNodeParser(elements, postProcessFlag);
                    Element* parsedEl = initialNodeParser.parseElement(node["nodes"][i]["initialNode"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(parsedEl));
                    dynamic_cast<ActivityNode*>(parsedEl)->setActivity(dynamic_cast<Activity*>(el));
                } else if (node["nodes"][i]["inputPin"]) {
                    InputPinParser inputPinParser(elements, postProcessFlag);
                    Element* parsedEl = inputPinParser.TypedElementParser::parseElement(node["nodes"][i]["inputPin"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(parsedEl));
                    dynamic_cast<ActivityNode*>(parsedEl)->setActivity(dynamic_cast<Activity*>(el));
                } else if (node["nodes"][i]["joinNode"]) {
                    JoinNodeParser joinNodeParser(elements, postProcessFlag);
                    Element* parsedEl = joinNodeParser.parseElement(node["nodes"][i]["joinNode"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(parsedEl));
                    dynamic_cast<ActivityNode*>(parsedEl)->setActivity(dynamic_cast<Activity*>(el));
                } else if (node["nodes"][i]["mergeNode"]) {
                    MergeNodeParser mergeNodeParser(elements, postProcessFlag);
                    Element* parsedEl = mergeNodeParser.parseElement(node["nodes"][i]["mergeNode"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(parsedEl));
                    dynamic_cast<ActivityNode*>(parsedEl)->setActivity(dynamic_cast<Activity*>(el));
                } else if (node["nodes"][i]["objectNode"]) {
                    ObjectNodeParser objectNodeParser(elements, postProcessFlag);
                    Element* parsedEl = objectNodeParser.parseElement(node["nodes"][i]["objectNode"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(parsedEl));
                    dynamic_cast<ActivityNode*>(parsedEl)->setActivity(dynamic_cast<Activity*>(el));
                } else if (node["nodes"][i]["outputPin"]) {
                    OutputPinParser outputPinParser(elements, postProcessFlag);
                    Element* parsedEl = outputPinParser.TypedElementParser::parseElement(node["nodes"][i]["outputPin"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(parsedEl));
                    dynamic_cast<ActivityNode*>(parsedEl)->setActivity(dynamic_cast<Activity*>(el));
                } else if (node["nodes"][i]["parameterNode"]) {
                    ParameterNodeParser parameterNodeParser(elements, postProcessFlag);
                    Element* parsedEl = parameterNodeParser.parseElement(node["nodes"][i]["parameterNode"]);
                    dynamic_cast<Activity*>(el)->nodes.push_back(dynamic_cast<ActivityNode*>(parsedEl));
                    dynamic_cast<ActivityNode*>(parsedEl)->setActivity(dynamic_cast<Activity*>(el));
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
                if (node["edges"][i]["controlFlow"]) {
                    ControlFlowParser controlFlowParser(elements, postProcessFlag);
                    Element* parsedEl = controlFlowParser.parseElement(node["edges"][i]["controlFlow"]);
                    dynamic_cast<Activity*>(el)->edges.push_back(dynamic_cast<ControlFlow*>(parsedEl));
                } else if (node["edges"][i]["objectFlow"]) {
                    ObjectFlowParser objectFlowParser(elements, postProcessFlag);
                    Element* parsedEl = objectFlowParser.parseElement(node["edges"][i]["objectFlow"]);
                    dynamic_cast<Activity*>(el)->edges.push_back(dynamic_cast<ObjectFlow*>(parsedEl));
                } else {
                    // TODO error
                }
            }
        } else {
            throw ElementParser::InvalidNodeTypeException(node["edges"].Mark().line, "sequence");
        }
    }

    return BehaviorParser::parseFeatures(node, el);
}

bool ActivityParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::ACTIVITY) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "activity";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = BehaviorParser::emit(emitter, el);

    if (!dynamic_cast<Activity*>(el)->nodes.empty()) {
        emitter << YAML::Key << "nodes";
        emitter << YAML::Value << YAML::BeginSeq;
        for(auto const& node : dynamic_cast<Activity*>(el)->nodes) {
            switch(node->getElementType()) {
                case ElementType::ACTION : {
                    ActionParser ap(elements, postProcessFlag);
                    ap.emit(emitter, node);
                    break;
                }
                case ElementType::CALL_BEHAVIOR_ACTION : {
                    CallBehaviorActionParser cbap(elements, postProcessFlag);
                    cbap.emit(emitter, node);
                    break;
                }
                case ElementType::DECISION_NODE : {
                    DecisionNodeParser dnp(elements, postProcessFlag);
                    dnp.emit(emitter, node);
                    break;
                }
                case ElementType::FINAL_NODE : {
                    FinalNodeParser fnp(elements, postProcessFlag);
                    fnp.emit(emitter, node);
                    break;
                }
                case ElementType::FORK_NODE : {
                    ForkNodeParser frknp(elements, postProcessFlag);
                    frknp.emit(emitter, node);
                    break;
                }
                case ElementType::INITIAL_NODE : {
                    InitialNodeParser inp(elements, postProcessFlag);
                    inp.emit(emitter, node);
                    break;
                }
                case ElementType::INPUT_PIN : {
                    InputPinParser ipp(elements, postProcessFlag);
                    ipp.emit(emitter, node);
                    break;
                }
                case ElementType::JOIN_NODE : {
                    JoinNodeParser jnp(elements, postProcessFlag);
                    jnp.emit(emitter, node);
                    break;
                }
                case ElementType::MERGE_NODE : {
                    MergeNodeParser mnp(elements, postProcessFlag);
                    mnp.emit(emitter, node);
                    break;
                }
                case ElementType::OBJECT_NODE : {
                    ObjectNodeParser onp(elements, postProcessFlag);
                    onp.emit(emitter, node);
                    break;
                }
                case ElementType::OUTPUT_PIN : {
                    OutputPinParser opp(elements, postProcessFlag);
                    opp.emit(emitter, node);
                    break;
                }
                case ElementType::PARAMETER_NODE : {
                    ParameterNodeParser pnp(elements, postProcessFlag);
                    pnp.emit(emitter, node);
                    break;
                }
                default : {
                    // TODO error
                }
            }
        }
        emitter << YAML::EndSeq;
    }

    if (!dynamic_cast<Activity*>(el)->edges.empty()) {
        emitter << YAML::Key << "edges";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& edge: dynamic_cast<Activity*>(el)->edges) {
            switch(edge->getElementType()) {
                case ElementType::CONTROL_FLOW : {
                    ControlFlowParser cfp(elements, postProcessFlag);
                    cfp.emit(emitter, edge);
                    break;
                }
                case ElementType::OBJECT_FLOW : {
                    ObjectFlowParser ofp(elements, postProcessFlag);
                    ofp.emit(emitter, edge);
                    break;
                }
            }
        }
        emitter << YAML::EndSeq;
    }

    if (el->getElementType() == ElementType::ACTIVITY) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

ActivityParser ActivityParser::createNewParser() {
    return ActivityParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}