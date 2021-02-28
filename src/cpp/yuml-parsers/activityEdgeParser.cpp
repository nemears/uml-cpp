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

            parseNowOrLater(guardID, el->uuid, &ActivityEdgeParser::setInstanceGuardLater);
        } else {
            if (dynamic_cast<ActivityEdge*>(el)->getSource() != NULL){
                if (dynamic_cast<ActivityEdge*>(el)->getSource()->isObjectNode()) {
                    if (dynamic_cast<ObjectNode*>(dynamic_cast<ActivityEdge*>(el)->getSource())->getType() != NULL) {
                        if (dynamic_cast<ObjectNode*>(dynamic_cast<ActivityEdge*>(el)->getSource())->getType()->isPrimitive()) {
                            // Literals
                            switch(dynamic_cast<PrimitiveType*>(dynamic_cast<ObjectNode*>(dynamic_cast<ActivityEdge*>(el)->getSource())->getType())->getPrimitiveType()) {
                                case PrimitiveType::Primitive::BOOL : {
                                    LiteralBool* lb = new LiteralBool;
                                    lb->setValue(node["guard"].as<bool>());
                                    dynamic_cast<ActivityEdge*>(el)->setGuard(lb);
                                }
                                case PrimitiveType::Primitive::INT : {
                                    LiteralInt* li = new LiteralInt;
                                    li->setValue(node["guard"].as<int>());
                                    dynamic_cast<ActivityEdge*>(el)->setGuard(li);
                                    break;
                                }
                                case PrimitiveType::Primitive::STRING : {
                                    LiteralString* ls = new LiteralString;
                                    ls->setValue(node["guard"].as<string>());
                                    dynamic_cast<ActivityEdge*>(el)->setGuard(ls);
                                    break;
                                }
                                case PrimitiveType::Primitive::REAL : {
                                    LiteralReal* lr = new LiteralReal;
                                    lr->setValue(node["guard"].as<double>());
                                    dynamic_cast<ActivityEdge*>(el)->setGuard(lr);
                                    break;
                                }
                                default : {
                                    // ERROR
                                    return false;
                                }
                            }
                        } else {
                            // instance
                            // Error?
                            return false;
                        }
                    }
                } else if (dynamic_cast<ActivityEdge*>(el)->getSource()->getElementType() == ElementType::DECISION_NODE) {
                    // get type from input to decision node
                    // This is when a guard is for a decision (proper use i think) but could use the field in UML 2.0 "decisionInputFlow" type ObjectFlow
                    // Current detection would just be slightly slower, and lossy if multiple object flows to decision node for some reason (don't know use case)
                    for (auto const& decisionIncomingEdge: dynamic_cast<ActivityEdge*>(el)->getSource()->incoming) {
                        if (decisionIncomingEdge->getElementType() == ElementType::OBJECT_FLOW) {
                            if (dynamic_cast<ObjectNode*>(decisionIncomingEdge->getSource())->getType()->isPrimitive()) {
                                switch(dynamic_cast<PrimitiveType*>(dynamic_cast<ObjectNode*>(decisionIncomingEdge->getSource())->getType())->getPrimitiveType()) {
                                    case PrimitiveType::Primitive::BOOL : {
                                        LiteralBool* lb = new LiteralBool;
                                        lb->setValue(node["guard"].as<bool>());
                                        dynamic_cast<ActivityEdge*>(el)->setGuard(lb);
                                        break;
                                    }
                                    case PrimitiveType::Primitive::INT : {
                                        LiteralInt* li = new LiteralInt;
                                        li->setValue(node["guard"].as<int>());
                                        dynamic_cast<ActivityEdge*>(el)->setGuard(li);
                                        break;
                                    }
                                    case PrimitiveType::Primitive::STRING : {
                                        LiteralString* ls = new LiteralString;
                                        ls->setValue(node["guard"].as<string>());
                                        dynamic_cast<ActivityEdge*>(el)->setGuard(ls);
                                        break;
                                    }
                                    case PrimitiveType::Primitive::REAL : {
                                        LiteralReal* lr = new LiteralReal;
                                        lr->setValue(node["guard"].as<double>());
                                        dynamic_cast<ActivityEdge*>(el)->setGuard(lr);
                                        break;
                                    }
                                    default : {
                                        // ERROR
                                        return false;
                                    }
                                }
                            }
                            break;
                        }
                        if (dynamic_cast<ActivityEdge*>(el)->getGuard() == 0) {
                            // Here we can see that we failed so signal for backwards parsing?
                            return false;
                        }
                    }
                } else {
                    // Cannot determine guard
                }
            } else {
                // TODO backwards parsing
            }
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