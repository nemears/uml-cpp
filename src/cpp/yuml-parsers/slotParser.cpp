#include "yuml-parsers/slotParser.h"

using namespace UML;

Element* SlotParser::createElement() {
    return new Slot;
}

bool SlotParser::parseFeatures(YAML::Node node, Element* el) {
    if (node["definingFeature"]) {
        string parsedId = node["definingFeature"].as<string>();

        if (UML::isValidUUID4(parsedId)) {
            boost::uuids::uuid definingFeatureId = boost::lexical_cast<boost::uuids::uuid>(parsedId);

            StructuralFeature* definingFeature = (StructuralFeature*)(*elements)[definingFeatureId];

            ((Slot*)el)->setDefiningFeature(definingFeature);
        }
    }
    if (node["value"]) {
        if (((StructuralFeature*)((Slot*) el)->getDefiningFeature())->getType() != NULL) {
            if (((StructuralFeature*)((Slot*) el)->getDefiningFeature())->getType()->isPrimitive()) {
                switch (((PrimitiveType*) ((StructuralFeature*)((Slot*) el)->getDefiningFeature())->getType())->getPrimitiveType()) {
                    case PrimitiveType::Primitive::STRING : {
                        string stringVal = node["value"].as<string>();
                        LiteralString* ls = new LiteralString;
                        ls->setValue(stringVal);
                        ((Slot *) el)->values.push_back(ls);
                        break;
                    }
                    case PrimitiveType::Primitive::INT : {
                        int intVal = node["value"].as<int>();
                        LiteralInt* li = new LiteralInt;
                        li->setValue(intVal);
                        ((Slot *) el)->values.push_back(li);
                        break;
                    }
                    case PrimitiveType::Primitive::REAL : {
                        double realVal = node["value"].as<double>();
                        LiteralReal* lr = new LiteralReal;
                        lr->setValue(realVal);
                        ((Slot *) el)->values.push_back(lr);
                        break;
                    }
                    case PrimitiveType::Primitive::BOOL : {
                        bool boolVal = node["value"].as<bool>();
                        LiteralBool* lb = new LiteralBool;
                        lb->setValue(boolVal);
                        ((Slot *) el)->values.push_back(lb);
                        break;
                    }
                    default : {
                        //TODO error
                        break;
                    }
                }
            } else {
                string parsedId = node["value"].as<string>();

                if (UML::isValidUUID4(parsedId)) {
                    boost::uuids::uuid valueId = boost::lexical_cast<boost::uuids::uuid>(parsedId);

                    InstanceSpecification* value = (InstanceSpecification*)(*elements)[valueId];

                    InstanceValue* instVal = new InstanceValue();
                    instVal->setInstance(value);

                    ((Slot*)el)->values.push_back(instVal);
                }
            }
        } else {
            // TODO ERROR
        }
    }

    return ElementParser::parseFeatures(node, el);
}

bool SlotParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::SLOT) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "slot";
        emitter << YAML::BeginMap;
    }

    bool ret = ElementParser::emit(emitter, el);

    if (((Slot*)el)->getDefiningFeature() != NULL) {
        emitter << YAML::Key << "definingFeature";
        emitter << YAML::Value << boost::lexical_cast<string>(((Slot*)el)->getDefiningFeature()->uuid);
    }

    if (!((Slot*)el)->values.empty()) {
        if (((Slot*)el)->getDefiningFeature()->getType() != NULL) {
            emitter << YAML::Key << "value";
            if (((Slot*)el)->getDefiningFeature()->getType()->isPrimitive()) {
                switch (((PrimitiveType*)((Slot*)el)->getDefiningFeature()->getType())->getPrimitiveType()) {
                    case PrimitiveType::Primitive::BOOL : {
                        emitter << YAML::Value << ((LiteralBool*)((Slot*)el)->values.front())->getValue();
                        break;
                    }
                    case PrimitiveType::Primitive::INT : {
                        emitter << YAML::Value << ((LiteralInt*)((Slot*)el)->values.front())->getValue();
                        break;
                    }
                    case PrimitiveType::Primitive::REAL : {
                        emitter << YAML::Value << ((LiteralReal*)((Slot*)el)->values.front())->getValue();
                        break;
                    }
                    case PrimitiveType::Primitive::STRING : {
                        emitter << YAML::Value << ((LiteralString*)((Slot*)el)->values.front())->getValue();
                        break;
                    }
                }
            } else {
                if (((Slot*)el)->values.front()->getElementType() == ElementType::INSTANCE_VALUE) {
                    if (((InstanceValue*)((Slot*)el)->values.front())->getInstance() != NULL) {
                        emitter << YAML::Value << boost::lexical_cast<string>(((InstanceValue*)((Slot*)el)->values.front())->getInstance()->uuid);
                    }
                } else {
                    // TODO Error
                }
            }
        } else {
            // TODO Error
        }
    }

    if (el->getElementType() == ElementType::SLOT) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}