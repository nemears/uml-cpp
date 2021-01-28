#include "yuml-parsers/typedElementParser.h"

bool TypedElementParser::parseFeatures(YAML::Node node, UML::Element* el) {
    if (node["type"]) {
        string parsedId = node["type"].as<string>();

        if (UML::isValidUUID4(parsedId)) {
            boost::uuids::uuid typeId = boost::lexical_cast<boost::uuids::uuid>(parsedId);

            UML::Type* type = (UML::Type*)(*elements)[typeId];

            ((UML::TypedElement*)el)->setType(type);
        } else {
            if (parsedId.compare("STRING") == 0) {
                UML::PrimitiveType* stringType = new UML::PrimitiveType;
                stringType->setPrimitiveType(UML::PrimitiveType::Primitive::STRING);
                ((UML::TypedElement*)el)->setType(stringType);
            } else if (parsedId.compare("INT") == 0) {
                UML::PrimitiveType* intType = new UML::PrimitiveType;
                intType->setPrimitiveType(UML::PrimitiveType::Primitive::INT);
                ((UML::TypedElement*)el)->setType(intType);
            } else if (parsedId.compare("REAL") == 0) {
                UML::PrimitiveType* realType = new UML::PrimitiveType;
                realType->setPrimitiveType(UML::PrimitiveType::Primitive::REAL);
                ((UML::TypedElement*)el)->setType(realType);
            } else if (parsedId.compare("BOOL") == 0) {
                UML::PrimitiveType* boolType = new UML::PrimitiveType;
                boolType->setPrimitiveType(UML::PrimitiveType::Primitive::BOOL);
                ((UML::TypedElement*)el)->setType(boolType);
            } else {
                // ERROR
                cerr << "Invalid type detected " << parsedId << '\n';
            }
        }
        
    }

    return NamedElementParser::parseFeatures(node, el);
}