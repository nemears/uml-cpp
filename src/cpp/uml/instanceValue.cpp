#include "uml/instanceValue.h"

using namespace UML;

InstanceSpecification*  InstanceValue::getInstance() {
    return instance;
}

void InstanceValue::setInstance(InstanceSpecification* inst) {
    instance = inst;
}

ElementType InstanceValue::getElementType() {
    return ElementType::INSTANCE_VALUE;
}

bool InstanceValue::isSubClassOf(ElementType eType) {
    bool ret = ValueSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::INSTANCE_VALUE;
    }

    return ret;
}