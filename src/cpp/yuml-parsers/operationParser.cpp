#include "yuml-parsers/operationParser.h"

Element* OperationParser::createElement() {
    return new Operation;
}

bool OperationParser::parseFeatures(YAML::Node node, Element* el) {

    if (node["methods"]) {
        if (node["methods"].IsSequence()) {
            for (std::size_t i=0; i<node["methods"].size(); i++) {
                if (node["methods"][i].IsMap()) {
                    if (node["methods"][i]["activity"]) {
                        ActivityParser activityParser(elements, postProcessFlag);
                        Element* parsedEl = activityParser.parseElement(node["methods"][i]["activity"]);
                        dynamic_cast<Operation*>(el)->getMethods().add(*dynamic_cast<Activity*>(parsedEl));
                        dynamic_cast<Behavior*>(parsedEl)->setSpecification(dynamic_cast<Operation*>(el));
                        for (auto const& param: dynamic_cast<Behavior*>(parsedEl)->getParameters()) {
                            param->setOperation(dynamic_cast<Operation*>(el));
                        }
                    } else if (node["methods"][i]["opaqueBehavior"]) {
                        OpaqueBehaviorParser opaqueBehaviorParser(elements, postProcessFlag);
                        Element* parsedEl = opaqueBehaviorParser.parseElement(node["methods"][i]["opaqueBehavior"]);
                        dynamic_cast<Operation*>(el)->getMethods().add(*dynamic_cast<OpaqueBehavior*>(parsedEl));
                        dynamic_cast<Behavior*>(parsedEl)->setSpecification(dynamic_cast<Operation*>(el));
                        for (auto const& param: dynamic_cast<Behavior*>(parsedEl)->getParameters()) {
                            param->setOperation(dynamic_cast<Operation*>(el));
                        }
                    } // TODO other types here
                } else if (node["methods"][i].IsScalar()) {
                    if (isValidUUID4(node["methods"][i].as<string>())) {
                        boost::uuids::uuid methodId = boost::lexical_cast<boost::uuids::uuid>(node["methods"][i].as<string>());

                        parseNowOrLater(methodId, el->getID(), node, &OperationParser::addMethodLater);
                    }
                }
            }
        } else {
            // Error
            throw InvalidNodeTypeException(node["parameters"].Mark().line, "sequence");
        }
    }

    if (node["type"]) {
        string parsedId = node["type"].as<string>();

        if (UML::isValidUUID4(parsedId)) {
            boost::uuids::uuid typeId = boost::lexical_cast<boost::uuids::uuid>(parsedId);

            Type* type = dynamic_cast<Type*>((*elements)[typeId]);

            dynamic_cast<TypedElement*>(el)->setType(type);
        } else {
            if (parsedId.compare("STRING") == 0) {
                PrimitiveType* stringType = new PrimitiveType;
                stringType->setPrimitiveType(PrimitiveType::Primitive::STRING);
                dynamic_cast<Operation*>(el)->setType(stringType);
            } else if (parsedId.compare("INT") == 0) {
                PrimitiveType* intType = new PrimitiveType;
                intType->setPrimitiveType(PrimitiveType::Primitive::INT);
                dynamic_cast<Operation*>(el)->setType(intType);
            } else if (parsedId.compare("REAL") == 0) {
                PrimitiveType* realType = new PrimitiveType;
                realType->setPrimitiveType(PrimitiveType::Primitive::REAL);
                dynamic_cast<Operation*>(el)->setType(realType);
            } else if (parsedId.compare("BOOL") == 0) {
                PrimitiveType* boolType = new PrimitiveType;
                boolType->setPrimitiveType(PrimitiveType::Primitive::BOOL);
                dynamic_cast<Operation*>(el)->setType(boolType);
            } else {
                // Error
                throw InvalidIdentifierException(node["type"].Mark().line, node["type"].as<string>());
            }
        }
        
    }

    return NamedElementParser::parseFeatures(node, el);
}

bool OperationParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::OPERATION) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "operation";
        emitter << YAML::BeginMap;
    }
    
    bool ret = NamedElementParser::emit(emitter, el);

    if (!dynamic_cast<Operation*>(el)->getMethods().empty()) {
        emitter << YAML::Key << "methods";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& method: dynamic_cast<Operation*>(el)->getMethods()) {
            switch(method->getElementType()) {
                case ElementType::ACTIVITY : {
                    ActivityParser ap(elements, postProcessFlag);
                    if (!ap.emit(emitter, method)) {
                        return false;
                    }
                    break;
                }
                case ElementType::OPAQUE_BEHAVIOR : {
                    OpaqueBehaviorParser op(elements, postProcessFlag);
                    if (!op.emit(emitter, method)) {
                        return false;
                    }
                    break;
                }
                default : {
                    throw AbstractTypeEmitException(method->getElementTypeString(), boost::lexical_cast<string>(method->getID()));
                }
            }
        }
        emitter << YAML::EndSeq;
    }

    if (dynamic_cast<Operation*>(el)->getType() != NULL) {
        emitter << YAML::Key << "type";
        if (dynamic_cast<Operation*>(el)->getType()->isPrimitive()) {
            switch (((PrimitiveType*)dynamic_cast<Operation*>(el)->getType())->getPrimitiveType()) {
                case PrimitiveType::Primitive::BOOL : {
                    emitter << YAML::Value << "BOOL";
                    break;
                }
                case PrimitiveType::Primitive::INT : {
                    emitter << YAML::Value << "INT";
                    break;
                }
                case PrimitiveType::Primitive::REAL : {
                    emitter << YAML::Value << "REAL";
                    break;
                }
                case PrimitiveType::Primitive::STRING : {
                    emitter << YAML::Value << "STRING";
                    break;
                }
                default : {
                    //TODO error
                    return false;
                }
            }
        } else {
            emitter << YAML::Value << boost::lexical_cast<string>(dynamic_cast<Operation*>(el)->getType()->getID());
        }
    }

    if (el->getElementType() == ElementType::OPERATION) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

OperationParser OperationParser::createNewParser() {
    return OperationParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}