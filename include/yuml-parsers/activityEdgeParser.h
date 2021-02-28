#ifndef ACTIVITYEDGEPARSER_H
#define ACTIVITYEDGEPARSER_H

#include "namedElementParser.h"
#include "uml/activityEdge.h"
#include "uml/activityNode.h"
#include "uml/instanceValue.h"
#include "uml/objectNode.h"
#include "uml/primitiveType.h"
#include "uml/literalBool.h"
#include "uml/literalInt.h"
#include "uml/literalReal.h"
#include "uml/literalString.h"
#include "uml/decisionNode.h"

class ActivityEdgeParser : public NamedElementParser {
    protected:
        bool parseFeatures(YAML::Node node, Element* el) override;
    public:
        bool emit(YAML::Emitter& emitter, Element* el) override;
        ActivityEdgeParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            NamedElementParser(elements, postParsers) {};
        static void setInstanceGuardLater(YAML::Node node, Element* edge, Element* instGuard) {
            InstanceValue* iv = new InstanceValue;
            iv->setInstance(dynamic_cast<InstanceSpecification*>(instGuard));
            dynamic_cast<ActivityEdge*>(edge)->setGuard(dynamic_cast<ValueSpecification*>(iv));
        };
        static void setDecisionNodeLiteralGuardLater(YAML::Node node, Element* edge, Element* decisionInputNode) {
            if (dynamic_cast<ObjectFlow*>(decisionInputNode)->getSource()->isObjectNode()) {
                if (dynamic_cast<ObjectNode*>(dynamic_cast<ObjectFlow*>(decisionInputNode)->getSource())->getType()->isPrimitive()) {
                    switch (dynamic_cast<PrimitiveType*>(dynamic_cast<ObjectNode*>(dynamic_cast<ObjectFlow*>(decisionInputNode)->getSource())->getType())->getPrimitiveType()) {
                        case PrimitiveType::Primitive::BOOL : {
                            LiteralBool* lb = new LiteralBool;
                            lb->setValue(node["guard"].as<bool>());
                            dynamic_cast<ActivityEdge*>(edge)->setGuard(lb);
                            break;
                        }
                        case PrimitiveType::Primitive::INT : {
                            LiteralInt* li = new LiteralInt;
                            li->setValue(node["guard"].as<int>());
                            dynamic_cast<ActivityEdge*>(edge)->setGuard(li);
                            break;
                        }
                        case PrimitiveType::Primitive::STRING : {
                            LiteralString* ls = new LiteralString;
                            ls->setValue(node["guard"].as<string>());
                            dynamic_cast<ActivityEdge*>(edge)->setGuard(ls);
                            break;
                        }
                        case PrimitiveType::Primitive::REAL : {
                            LiteralReal* lr = new LiteralReal;
                            lr->setValue(node["guard"].as<double>());
                            dynamic_cast<ActivityEdge*>(edge)->setGuard(lr);
                            break;
                        }
                        default : {
                            // ERROR
                        }
                    }
                }
            } else {
                // ERROR
            }
        }
};

#endif