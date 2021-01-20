#include "headers/instanceValue.h"

using namespace UML;

InstanceSpecification*  InstanceValue::getInstance() {
    return instance;
}

void InstanceValue::setInstance(InstanceSpecification* inst) {
    instance = inst;
}