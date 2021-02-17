#include "yuml-parsers/typedElementParser.h"

using namespace UML;

bool TypedElementParser::parseFeatures(YAML::Node node, Element* el) {
    
    bool ret = parseTypeFeatures(node, el);

    if(!NamedElementParser::parseFeatures(node, el)) {
        ret = false;
    }

    return ret;
}

bool TypedElementParser::parseTypeFeatures(YAML::Node node, Element* el) {
    if (node["type"]) {
        string parsedId = node["type"].as<string>();

        if (UML::isValidUUID4(parsedId)) {
            boost::uuids::uuid typeId = boost::lexical_cast<boost::uuids::uuid>(parsedId);

            // check if null
            // if null we make a flag for backwards parsing
            if((*elements)[typeId] == 0) {

                // check if struct created
                if ((*postProcessFlag)[typeId] == 0) {
                    list<boost::uuids::uuid>* eList = new list<boost::uuids::uuid>;
                    list<void(*)(Element*, Element*)>* fList = new list<void(*)(Element*, Element*)>;;
                    PostParser* postParser  =  new PostParser{*eList, *fList};
                    (*postProcessFlag)[typeId] = postParser;
                } 

                // add flag with function pointer
                (*postProcessFlag)[typeId]->otherEls.push_back(el->uuid);
                (*postProcessFlag)[typeId]->applyOnEl.push_back(&TypedElementParser::setTypeLater);
            } else {

                // set the type
                Type* type = dynamic_cast<Type*>((*elements)[typeId]);

                dynamic_cast<TypedElement*>(el)->setType(type);
            }
        } else {
            if (parsedId.compare("STRING") == 0) {
                PrimitiveType* stringType = new PrimitiveType;
                stringType->setPrimitiveType(UML::PrimitiveType::Primitive::STRING);
                dynamic_cast<TypedElement*>(el)->setType(stringType);
            } else if (parsedId.compare("INT") == 0) {
                PrimitiveType* intType = new PrimitiveType;
                intType->setPrimitiveType(UML::PrimitiveType::Primitive::INT);
                dynamic_cast<TypedElement*>(el)->setType(intType);
            } else if (parsedId.compare("REAL") == 0) {
                UML::PrimitiveType* realType = new UML::PrimitiveType;
                realType->setPrimitiveType(UML::PrimitiveType::Primitive::REAL);
                dynamic_cast<TypedElement*>(el)->setType(realType);
            } else if (parsedId.compare("BOOL") == 0) {
                UML::PrimitiveType* boolType = new UML::PrimitiveType;
                boolType->setPrimitiveType(UML::PrimitiveType::Primitive::BOOL);
                dynamic_cast<TypedElement*>(el)->setType(boolType);
            } else {
                // ERROR
                throw InvalidIdentifierException(node["type"].Mark().line, node["type"].as<string>());
            }
        }
        
    }

    return true;
}

bool TypedElementParser::emit(YAML::Emitter& emitter, Element* el) {
    
    bool ret = emitType(emitter, el);

    if (!NamedElementParser::emit(emitter, el)) {
        ret = false;
    }

    return ret;
}

bool TypedElementParser::emitType(YAML::Emitter& emitter, Element* el) {
    if (dynamic_cast<TypedElement*>(el)->getType() != NULL) {
        emitter << YAML::Key << "type";
        if (dynamic_cast<TypedElement*>(el)->getType()->isPrimitive()) {
            switch (((PrimitiveType*)dynamic_cast<TypedElement*>(el)->getType())->getPrimitiveType()) {
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
            emitter << YAML::Value << boost::lexical_cast<string>(dynamic_cast<TypedElement*>(el)->getType()->uuid);
        }
    }

    return true;
}