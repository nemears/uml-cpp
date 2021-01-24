#include "headers/operationParser.h"

Element* OperationParser::createElement() {
    return new Operation;
}

bool OperationParser::parseFeatures(YAML::Node node, Element* el) {

    if (node["parameters"]) {
        if (node["parameters"].IsSequence()) {
            for (std::size_t i=0; i<node["parameters"].size(); i++) {
                ParameterParser parameterParser(elements);
                Element* parsedEl = parameterParser.parseElement(node["parameters"][i]["parameter"]);
                ((Operation*) el)->parameters.push_back((Parameter*) parsedEl);
            }
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
        }
    }

    if (node["type"]) {
        string parsedId = node["type"].as<string>();

        if (UML::isValidUUID4(parsedId)) {
            boost::uuids::uuid typeId = boost::lexical_cast<boost::uuids::uuid>(parsedId);

            Type* type = (Type*)(*elements)[typeId];

            ((TypedElement*)el)->setType(type);
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
                // ERROR
                cerr << "Invalid type detected " << parsedId << '\n';
            }
        }
        
    }

    return NamedElementParser::parseFeatures(node, el);
}