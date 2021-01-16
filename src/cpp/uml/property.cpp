#include "headers/property.h"
#include "headers/primitiveType.h"

using namespace UML;

void Property::setDefaultValue(ValueSpecification* val) {

    // TODO add checks to see if defaultValue corresponds with type
    if (type == NULL) {
        throw invalidValueException;
    }

    if (type->isPrimitive()) {
        Type* valType = val->getType();
        if (!(valType->isPrimitive())) {
            throw invalidValueException;
        }
        if (((PrimitiveType*)val->getType())->getPrimitiveType() != ((PrimitiveType*) type)->getPrimitiveType()) {
            throw invalidValueException;
        }
    }

    // TODO check that val type is same as property type if not primitive

    defaultValue = val;
}

ValueSpecification* Property::getDefaultValue() {
    return defaultValue;
}