#include "headers/propertyParser.h"

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
                    LiteralString ls;
                    ls.setValue(stringVal);
                }
                case PrimitiveType::Primitive::INT : {
                    int intVal = node["defaultValue"].as<int>();
                    LiteralInt li;
                    li.setValue(intVal);
                }
                case PrimitiveType::Primitive::REAL : {
                    double realVal = node["defaultValue"].as<double>();
                    LiteralReal lr;
                    lr.setValue(realVal);
                }
                case PrimitiveType::Primitive::BOOL : {
                    bool boolBal = node["defaultValue"].as<bool>();
                    LiteralBool lb;
                    lb.setValue(boolBal);
                }
            }
        } else {
            // instances
        }
    }

    return ret;
}