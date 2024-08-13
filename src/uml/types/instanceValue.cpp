#include "uml/uml-stable.h"

using namespace UML;

void InstanceValue::referenceErased(ID id) {
    ValueSpecification::referenceErased(id);
    eraseFromSet(id, m_instance);
}

Singleton<InstanceSpecification, InstanceValue>& InstanceValue::getInstanceSingleton() {
    return m_instance;
}

InstanceValue::InstanceValue() : Element(ElementType::INSTANCE_VALUE) {
    
}

InstanceValue::~InstanceValue() {
    
}

InstanceSpecificationPtr  InstanceValue::getInstance() const {
    return m_instance.get();
}

void InstanceValue::setInstance(InstanceSpecificationPtr inst) {
    m_instance.set(inst);
}

void InstanceValue::setInstance(InstanceSpecification& inst) {
    m_instance.set(inst);
}

void InstanceValue::setInstance(ID id) {
    m_instance.set(id);
}

bool InstanceValue::is(ElementType eType) const {
    bool ret = ValueSpecification::is(eType);

    if (!ret) {
        ret = eType == ElementType::INSTANCE_VALUE;
    }

    return ret;
}
