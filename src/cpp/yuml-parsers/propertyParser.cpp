#include "yuml-parsers/propertyParser.h"

using namespace UML;

Element* PropertyParser::createElement(){
    return new Property;
}

bool PropertyParser::parseFeatures(YAML::Node node, UML::Element* el) {

    bool ret = TypedElementParser::parseFeatures(node, el);

    if (node["defaultValue"]) {
        if (((Property*) el)->getType() != NULL) {
            if (((Property*) el)->getType()->isPrimitive()) {
                switch (((PrimitiveType*) ((Property*)el)->getType())->getPrimitiveType()) {
                    case PrimitiveType::Primitive::STRING : {
                        string stringVal = node["defaultValue"].as<string>();
                        LiteralString* ls = new LiteralString;
                        ls->setValue(stringVal);
                        ((Property *) el)->setDefaultValue(ls);
                        break;
                    }
                    case PrimitiveType::Primitive::INT : {
                        int intVal = node["defaultValue"].as<int>();
                        LiteralInt* li = new LiteralInt;
                        li->setValue(intVal);
                        ((Property *) el)->setDefaultValue(li);
                        break;
                    }
                    case PrimitiveType::Primitive::REAL : {
                        double realVal = node["defaultValue"].as<double>();
                        LiteralReal* lr = new LiteralReal;
                        lr->setValue(realVal);
                        ((Property *) el)->setDefaultValue(lr);
                        break;
                    }
                    case PrimitiveType::Primitive::BOOL : {
                        bool boolVal = node["defaultValue"].as<bool>();
                        LiteralBool* lb = new LiteralBool;
                        lb->setValue(boolVal);
                        ((Property *) el)->setDefaultValue(lb);
                        break;
                    }
                    default : {
                        //TODO error
                        break;
                    }
                }
            } else {
                // instances
                string parsedId = node["defaultValue"].as<string>();
                if (isValidUUID4(parsedId)) {
                    boost::uuids::uuid typeId = boost::lexical_cast<boost::uuids::uuid>(parsedId);
                    InstanceSpecification* defaultVal = (InstanceSpecification*) (*elements)[typeId];
                    InstanceValue* instanceVal = new InstanceValue;
                    instanceVal->setInstance(defaultVal);
                    ((Property*) el)->setDefaultValue(instanceVal);
                }
            }
        } else {
            // TODO throw error
        }
    }

    return ret;
}

bool PropertyParser::emit(YAML::Emitter& emitter, Element* el) {

    if (el->getElementType() == ElementType::PROPERTY) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "property";
        emitter << YAML::Value << YAML::BeginMap;
    }
    bool ret = TypedElementParser::emit(emitter, el);

    if (((Property*) el)->getDefaultValue() != NULL) {
        emitter << YAML::Key << "defaultValue";
        if (((Property*)el)->getType() != NULL) {
            if (((Property*)el)->getType()->isPrimitive()) {
                switch (((PrimitiveType*)((Property*)el)->getType())->getPrimitiveType()) {
                    case PrimitiveType::Primitive::BOOL : {
                        emitter << YAML::Value << ((LiteralBool*)((Property*)el)->getDefaultValue())->getValue();
                        break;
                    }
                    case PrimitiveType::Primitive::INT : {
                        emitter << YAML::Value << ((LiteralInt*)((Property*)el)->getDefaultValue())->getValue();
                        break;
                    }
                    case PrimitiveType::Primitive::REAL : {
                        emitter << YAML::Value << ((LiteralReal*)((Property*)el)->getDefaultValue())->getValue();
                        break;
                    }
                    case PrimitiveType::Primitive::STRING : {
                        emitter << YAML::Value << ((LiteralString*)((Property*)el)->getDefaultValue())->getValue();
                        break;
                    }
                    default : {
                        // TODO ERROR
                    }
                }
            } else {
                emitter << YAML::Value << boost::lexical_cast<string>(((InstanceValue*)((Property*) el)->getDefaultValue())->getInstance()->uuid);
            }
        } else {
            // TODO error
        }
    }

    if (el->getElementType() == ElementType::PROPERTY) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}