#include "yuml-parsers/propertyParser.h"

using namespace UML;

Element* PropertyParser::createElement(){
    return new Property;
}

bool PropertyParser::parseFeatures(YAML::Node node, UML::Element* el) {

    bool ret = TypedElementParser::parseFeatures(node, el);

    if (node["defaultValue"]) {

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
        }
    }

    return ret;
}

bool PropertyParser::emit(YAML::Emitter& emitter, Element* el) {

    if (el->getElementType() == ElementType::PROPERTY) {
        emitter << YAML::BeginMap;
        // TODO do TypedElementParser::emit and call it here
        if (((Property*) el)->getDefaultValue() != NULL) {
            // TODO do value emit
        }
    }
}