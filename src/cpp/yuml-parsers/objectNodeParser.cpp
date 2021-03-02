#include "yuml-parsers/objectNodeParser.h"

Element* ObjectNodeParser::createElement() {
    theEl = new ObjectNode;
    return theEl;
}

bool ObjectNodeParser::parseFeatures(YAML::Node node, Element* el) {
    bool ret = NamedElementParser::parseFeatures(node, el);

    if(!parseObjectNodeFeatures(node, el)) {
        ret = false;
    }

    return ret;
}

bool ObjectNodeParser::parseObjectNodeFeatures(YAML::Node node, Element* el) {
    
    bool ret = parseTypeFeatures(node, el);

    if (!parseActivityNodeFeatures(node, el)) {
        ret = false;
    }

    if (node["upperBound"]) {
        if (dynamic_cast<ObjectNode*>(el)->getType() != NULL) {
            if (dynamic_cast<ObjectNode*>(el)->getType()->isPrimitive()) {
                switch (((PrimitiveType*) dynamic_cast<ObjectNode*>(el)->getType())->getPrimitiveType()) {
                    case PrimitiveType::Primitive::STRING : {
                        string stringVal = node["upperBound"].as<string>();
                        LiteralString* ls = new LiteralString;
                        ls->setValue(stringVal);
                        dynamic_cast<ObjectNode*>(el)->setUpperBound(ls);
                        break;
                    }
                    case PrimitiveType::Primitive::INT : {
                        int intVal = node["upperBound"].as<int>();
                        LiteralInt* li = new LiteralInt;
                        li->setValue(intVal);
                        dynamic_cast<ObjectNode*>(el)->setUpperBound(li);
                        break;
                    }
                    case PrimitiveType::Primitive::REAL : {
                        double realVal = node["upperBound"].as<double>();
                        LiteralReal* lr = new LiteralReal;
                        lr->setValue(realVal);
                        dynamic_cast<ObjectNode*>(el)->setUpperBound(lr);
                        break;
                    }
                    case PrimitiveType::Primitive::BOOL : {
                        bool boolVal = node["upperBound"].as<bool>();
                        LiteralBool* lb = new LiteralBool;
                        lb->setValue(boolVal);
                        dynamic_cast<ObjectNode*>(el)->setUpperBound(lb);
                        break;
                    }
                    default : {
                        //TODO error
                        break;
                    }
                }
            } else {
                if (node["upperBound"].IsScalar()) {
                    // instances
                    string parsedId = node["upperBound"].as<string>();
                    if (isValidUUID4(parsedId)) {
                        boost::uuids::uuid typeId = boost::lexical_cast<boost::uuids::uuid>(parsedId);
                        InstanceSpecification* defaultVal = dynamic_cast<InstanceSpecification*>((*TypedElementParser::elements)[typeId]);
                        InstanceValue* instanceVal = new InstanceValue;
                        instanceVal->setInstance(defaultVal);
                        dynamic_cast<ObjectNode*>(el)->setUpperBound(instanceVal);
                    }
                } else if (node["upperBound"].IsMap()) {
                    if (node["upperBound"]["expression"]) {
                        ExpressionParser ep(TypedElementParser::elements, TypedElementParser::postProcessFlag);
                        Element* parsedEl = ep.parseElement(node["upperBound"]["expression"]);
                        dynamic_cast<ObjectNode*>(el)->setUpperBound(dynamic_cast<ValueSpecification*>(parsedEl));
                    }
                } else {
                    // TODO throw error
                    ret = false
                }
            }
        } else {
            // TODO throw error
            ret = false;
        }
    }

    return ret;
}

bool ObjectNodeParser::emit(YAML::Emitter& emitter, Element* el) {

    if (el->getElementType() == ElementType::OBJECT_NODE) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "objectNode";
        emitter << YAML::BeginMap;
    }

    bool ret = NamedElementParser::emit(emitter, el);
    if(!emitObjectNode(emitter, el)) {
        ret = false;
    }

    if (el->getElementType() == ElementType::OBJECT_NODE) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

bool ObjectNodeParser::emitObjectNode(YAML::Emitter& emitter, Element* el) {
        
    bool ret = emitType(emitter, el);

    if (!emitActivityNode(emitter, el)) {
        ret = false;
    }

    return ret;
}

ObjectNodeParser ObjectNodeParser::createNewParser() {
    return ObjectNodeParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}