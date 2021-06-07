#include "uml/instanceValue.h"

using namespace UML;

InstanceSpecification*  InstanceValue::getInstance() {
    if (!m_instanceID.isNull()) {
        if (!m_instancePtr) {
            m_instancePtr = &m_manager->get<InstanceSpecification>(m_instanceID);
        }
        return m_instancePtr;
    }
    return 0;
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

bool InstanceValue::isSubClassOf(ElementType eType) {
    bool ret = ValueSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::INSTANCE_VALUE;
    }

    return ret;
}