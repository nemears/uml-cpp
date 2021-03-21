#include "yuml-parsers/slotParser.h"

using namespace UML;

Element* SlotParser::createElement() {
    return new Slot;
}

bool SlotParser::parseFeatures(YAML::Node node, Element* el) {

    bool postParseDefiningFeature = false;

    if (node["definingFeature"]) {
        string parsedId = node["definingFeature"].as<string>();

        if (UML::isValidUUID4(parsedId)) {
            boost::uuids::uuid definingFeatureId = boost::lexical_cast<boost::uuids::uuid>(parsedId);

            postParseDefiningFeature = !parseNowOrLater(definingFeatureId, el->getID(), node, &SlotParser::setDefiningFeatureLater);
        }
    }
    if (node["value"]) {
        bool skipValueParseNow = false;
        if (((Slot*) el)->getDefiningFeature() == NULL) {
            // Error
            if (!postParseDefiningFeature) {
                throw ((Slot*)el)->nullDefiningFeatureException;
            }
            skipValueParseNow = true;
        }
        if (!skipValueParseNow) {
            if (((StructuralFeature*)((Slot*) el)->getDefiningFeature())->getType() != NULL) {
                if (((StructuralFeature*)((Slot*) el)->getDefiningFeature())->getType()->isPrimitive()) {
                    switch (((PrimitiveType*) ((StructuralFeature*)((Slot*) el)->getDefiningFeature())->getType())->getPrimitiveType()) {
                        case PrimitiveType::Primitive::STRING : {
                            string stringVal = node["value"].as<string>();
                            LiteralString* ls = new LiteralString;
                            ls->setValue(stringVal);
                            ((Slot *) el)->getValues().add(*ls);
                            el->getOwnedElements().add(*ls);
                            ls->setOwner(el);
                            break;
                        }
                        case PrimitiveType::Primitive::INT : {
                            int intVal = node["value"].as<int>();
                            LiteralInt* li = new LiteralInt;
                            li->setValue(intVal);
                            ((Slot *) el)->getValues().add(*li);
                            el->getOwnedElements().add(*li);
                            li->setOwner(el);
                            break;
                        }
                        case PrimitiveType::Primitive::REAL : {
                            double realVal = node["value"].as<double>();
                            LiteralReal* lr = new LiteralReal;
                            lr->setValue(realVal);
                            ((Slot *) el)->getValues().add(*lr);
                            el->getOwnedElements().add(*lr);
                            lr->setOwner(el);
                            break;
                        }
                        case PrimitiveType::Primitive::BOOL : {
                            bool boolVal = node["value"].as<bool>();
                            LiteralBool* lb = new LiteralBool;
                            lb->setValue(boolVal);
                            ((Slot *) el)->getValues().add(*lb);
                            el->getOwnedElements().add(*lb);
                            lb->setOwner(el);
                            break;
                        }
                        default : {
                            // should never get here
                            throw InvalidIdentifierException(node["value"].Mark().line, node["value"].as<string>());
                        }
                    }
                } else {
                    parseNonPimitiveValueFeatures(node, el);
                }
            } else {
                // Error
                throw ((Slot*)el)->getDefiningFeature()->invalidValueException;
            }
        } else {
            // so we were told to skip, if we can get a uuid from value mark it is not an error, it is backwards parsing
            parseNonPimitiveValueFeatures(node, el);
        }
    }

    return ElementParser::parseFeatures(node, el);
}

void SlotParser::parseNonPimitiveValueFeatures(YAML::Node node, Element* el) {
    if (node["value"].IsScalar()) {
        string parsedId = node["value"].as<string>();

        if (UML::isValidUUID4(parsedId)) {
            boost::uuids::uuid valueId = boost::lexical_cast<boost::uuids::uuid>(parsedId);

            parseNowOrLater(valueId, el->getID(), node, &SlotParser::setInstanceValueLater);
        }
    } else if (node["value"].IsMap()) {
        if (node["value"]["expression"]) {
            ExpressionParser ep(elements, postProcessFlag);
            Element* parsedEl = ep.parseElement(node["value"]["expression"]);
            dynamic_cast<Slot*>(el)->getValues().add(*dynamic_cast<ValueSpecification*>(parsedEl));
        }
    }
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
        emitter << YAML::Value << boost::lexical_cast<string>(((Slot*)el)->getDefiningFeature()->getID());
    }

    if (!((Slot*)el)->getValues().empty()) {
        if (((Slot*)el)->getDefiningFeature()->getType() != NULL) {
            emitter << YAML::Key << "value";
            if (((Slot*)el)->getDefiningFeature()->getType()->isPrimitive()) {
                switch (((PrimitiveType*)((Slot*)el)->getDefiningFeature()->getType())->getPrimitiveType()) {
                    case PrimitiveType::Primitive::BOOL : {
                        emitter << YAML::Value << ((LiteralBool*)((Slot*)el)->getValues().front())->getValue();
                        break;
                    }
                    case PrimitiveType::Primitive::INT : {
                        emitter << YAML::Value << ((LiteralInt*)((Slot*)el)->getValues().front())->getValue();
                        break;
                    }
                    case PrimitiveType::Primitive::REAL : {
                        emitter << YAML::Value << ((LiteralReal*)((Slot*)el)->getValues().front())->getValue();
                        break;
                    }
                    case PrimitiveType::Primitive::STRING : {
                        emitter << YAML::Value << ((LiteralString*)((Slot*)el)->getValues().front())->getValue();
                        break;
                    }
                }
            } else {
                if (((Slot*)el)->getValues().front()->getElementType() == ElementType::INSTANCE_VALUE) {
                    if (((InstanceValue*)((Slot*)el)->getValues().front())->getInstance() != NULL) {
                        emitter << YAML::Value << boost::lexical_cast<string>(((InstanceValue*)((Slot*)el)->getValues().front())->getInstance()->getID());
                    }
                } else {
                    // Error
                    throw AbstractTypeEmitException(((Slot*)el)->getValues().front()->getElementTypeString(), boost::lexical_cast<string>(((Slot*)el)->getValues().front()->getID()));
                }
            }
        } else {
            // Error
            throw(((Slot*)el)->getDefiningFeature()->invalidValueException);
        }
    }

    if (el->getElementType() == ElementType::SLOT) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

SlotParser SlotParser::createNewParser() {
    return SlotParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}