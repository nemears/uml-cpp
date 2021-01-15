#include "headers/property.h"

using namespace UML;

void Property::setDefaultValue(ValueSpecification* val) {

    // TODO add checks to see if defaultValue corresponds with type

    defaultValue = val;
}

ValueSpecification* Property::getDefaultValue() {
    return defaultValue;
}