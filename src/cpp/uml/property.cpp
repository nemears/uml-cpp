#include "headers/property.h"

using namespace UML;

void Property::setDefaultValue(ValueSpecification* val) {
    defaultValue = val;
}

ValueSpecification* Property::getDefaultValue() {
    return defaultValue;
}