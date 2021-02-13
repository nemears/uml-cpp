#include "yuml-parsers/operationParser.h"

Element* OperationParser::createElement() {
    return new Operation;
}

bool OperationParser::parseFeatures(YAML::Node node, Element* el) {

    if (node["parameters"]) {
        if (node["parameters"].IsSequence()) {
            for (std::size_t i=0; i<node["parameters"].size(); i++) {
                ParameterParser parameterParser(elements);
                Element* parsedEl = parameterParser.parseElement(node["parameters"][i]["parameter"]);
                ((Operation*) el)->parameters.push_back(dynamic_cast<Parameter*>(parsedEl));
            }
        } else {
            // Error
            throw InvalidNodeTypeException(node["parameters"].Mark().line, "sequence");
        }
    }

    if (node["methods"]) {
        if (node["methods"].IsSequence()) {
            for (std::size_t i=0; i<node["methods"].size(); i++) {
                if (node["methods"][i]["opaqueBehavior"]) {
                    OpaqueBehaviorParser opaqueBehaviorParser(elements);
                    Element* parsedEl = opaqueBehaviorParser.parseElement(node["methods"][i]["opaqueBehavior"]);
                    ((Operation*) el)->methods.push_back((OpaqueBehavior*) parsedEl);
                }

                // TODO other types here
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

            Type* type = (Type*)(*elements)[typeId];

            dynamic_cast<TypedElement*>(el)->setType(type);
        } else {
            if (parsedId.compare("STRING") == 0) {
                PrimitiveType* stringType = new PrimitiveType;
                stringType->setPrimitiveType(PrimitiveType::Primitive::STRING);
                ((Operation*)el)->setType(stringType);
            } else if (parsedId.compare("INT") == 0) {
                PrimitiveType* intType = new PrimitiveType;
                intType->setPrimitiveType(PrimitiveType::Primitive::INT);
                ((Operation*)el)->setType(intType);
            } else if (parsedId.compare("REAL") == 0) {
                PrimitiveType* realType = new PrimitiveType;
                realType->setPrimitiveType(PrimitiveType::Primitive::REAL);
                ((Operation*)el)->setType(realType);
            } else if (parsedId.compare("BOOL") == 0) {
                PrimitiveType* boolType = new PrimitiveType;
                boolType->setPrimitiveType(PrimitiveType::Primitive::BOOL);
                ((Operation*)el)->setType(boolType);
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

    if (!((Operation*)el)->parameters.empty()) {
        emitter << YAML::Key << "parameters";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& parameter: ((Operation*)el)->parameters) {
            ParameterParser pp(elements);
            if (!pp.emit(emitter, parameter)) {
                return false;
            }
        }
        emitter << YAML::EndSeq;
    }

    if (!((Operation*) el)->methods.empty()) {
        emitter << YAML::Key << "methods";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& method: ((Operation*)el)->methods) {
            switch(method->getElementType()) {
                case ElementType::OPAQUE_BEHAVIOR : {
                    OpaqueBehaviorParser op(elements);
                    if (!op.emit(emitter, method)) {
                        return false;
                    }
                    break;
                }
                default : {
                    throw AbstractTypeEmitException(method->getElementTypeString(), boost::lexical_cast<string>(method->uuid));
                }
            }
        }
        emitter << YAML::EndSeq;
    }

    if (((Operation*)el)->getType() != NULL) {
        emitter << YAML::Key << "type";
        if (((Operation*)el)->getType()->isPrimitive()) {
            switch (((PrimitiveType*)((Operation*)el)->getType())->getPrimitiveType()) {
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
            emitter << YAML::Value << boost::lexical_cast<string>(((Operation*) el)->getType()->uuid);
        }
    }

    if (el->getElementType() == ElementType::OPERATION) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}