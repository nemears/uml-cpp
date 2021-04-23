#include "uml/primitiveType.h"

using namespace UML;

void PrimitiveType::setPrimitiveType(Primitive primitive) {
    this->primitiveType = primitive;
}

PrimitiveType::Primitive PrimitiveType::getPrimitiveType() {
    return this->primitiveType;
}

ElementType PrimitiveType::getElementType() {
    return ElementType::PRIMITIVE_TYPE;
}

string PrimitiveType::getPrimitiveTypeString() {
    switch(primitiveType) {
        case (Primitive::STRING) : {
            return "STRING";
            break;
        }
        case (Primitive::INT) : {
            return "INT";
            break;
        }
        case (Primitive::REAL) : {
            return "REAL";
            break;
        }
        case (Primitive::BOOL) : {
            return "BOOL";
            break;
        }
        case (Primitive::NONE) : {
            return "NONE";
            break;
        }
    }
    return "NONE";
}

void PrimitiveType::setPrimitiveTypeString(const string& primitiveString) {
    if (primitiveString.compare("STRING") == 0) {
        primitiveType = Primitive::STRING;
    } else if (primitiveString.compare("INT") == 0) {
        primitiveType = Primitive::INT;
    } else if (primitiveString.compare("REAL") == 0) {
        primitiveType = Primitive::REAL;
    } else if (primitiveString.compare("BOOL") == 0) {
        primitiveType = Primitive::BOOL;
    } else if (primitiveString.compare("NONE") == 0) {
        primitiveType = Primitive::NONE;
    }
}

bool PrimitiveType::isSubClassOf(ElementType eType) {
    bool ret = DataType::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PRIMITIVE_TYPE;
    }

    return ret;
}