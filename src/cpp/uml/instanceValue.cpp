#include "uml/instanceValue.h"
#include "uml/instanceSpecification.h"
#include "uml/universalFunctions.h"

using namespace UML;

InstanceSpecification*  InstanceValue::getInstance() {
    return universalGet<InstanceSpecification>(m_instanceID, m_instancePtr, m_manager);
}

void InstanceValue::setInstance(InstanceSpecification* inst) {
    if (inst) {
        m_instanceID = inst->getID();
    } else {
        m_instanceID = ID::nullID();
    }

    if (!m_manager) {
        m_instancePtr = inst;
    } else {
        m_instancePtr = 0;
    }
}

ElementType InstanceValue::getElementType() const {
    return ElementType::INSTANCE_VALUE;
}

bool InstanceValue::isSubClassOf(ElementType eType) const {
    bool ret = ValueSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::INSTANCE_VALUE;
    }

    return ret;
}