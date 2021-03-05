#include "yuml-parsers/createObjectActionParser.h"

Element* CreateObjectActionParser::createElement() {
    theEl = new CreateObjectAction;
    return theEl;
}

bool CreateObjectActionParser::parseFeatures(YAML::Node node, Element* el) {
    bool ret = ActionParser::parseFeatures(node, el);

    if (node["classifier"]) {
        if (node["classifier"].IsScalar()) {
            if (isValidUUID4(node["classifier"].as<string>())) {
                boost::uuids::uuid classifierID = boost::lexical_cast<boost::uuids::uuid>(node["classifier"].as<string>());

                parseNowOrLater(classifierID, el->uuid, node["classifier"], &CreateObjectActionParser::setClassifierLater);
            } else if (node["classifier"].as<string>().compare("BOOL") == 0) {
                PrimitiveType* boolType = new PrimitiveType;
                boolType->setPrimitiveType(PrimitiveType::Primitive::BOOL);
                dynamic_cast<CreateObjectAction*>(el)->setClassifier(boolType);
            } else if (node["classifier"].as<string>().compare("INT") == 0) {
                PrimitiveType* intType = new PrimitiveType;
                intType->setPrimitiveType(PrimitiveType::Primitive::INT);
                dynamic_cast<CreateObjectAction*>(el)->setClassifier(intType);
            } else if (node["classifier"].as<string>().compare("REAL") == 0) {
                PrimitiveType* realType = new PrimitiveType;
                realType->setPrimitiveType(PrimitiveType::Primitive::REAL);
                dynamic_cast<CreateObjectAction*>(el)->setClassifier(realType);
            } else if (node["classifier"].as<string>().compare("STRING") == 0) {
                PrimitiveType* strType = new PrimitiveType;
                strType->setPrimitiveType(PrimitiveType::Primitive::STRING);
                dynamic_cast<CreateObjectAction*>(el)->setClassifier(strType);
            } else {
                // error
                return false;
            }
        }
    }

    return ret;
}

bool CreateObjectActionParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::CREATE_OBJECT_ACTION) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "createObjectAction";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = ActionParser::emit(emitter, el);

    if (el->getElementType() == ElementType::CREATE_OBJECT_ACTION) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}