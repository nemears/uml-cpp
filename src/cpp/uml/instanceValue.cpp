#include "uml/instanceValue.h"
#include "uml/instanceSpecification.h"
#include "uml/uml-stable.h"

using namespace UML;

void InstanceValue::referencingReleased(ID id) {
    ValueSpecification::referencingReleased(id);
    m_instance.release(id);
}

void InstanceValue::referenceReindexed(ID oldID, ID newID) {
    ValueSpecification::referenceReindexed(oldID, newID);
    m_instance.reindex(oldID, newID);
}

void InstanceValue::referenceErased(ID id) {
    ValueSpecification::referenceErased(id);
    m_instance.eraseElement(id);
}

Set<InstanceSpecification, InstanceValue>& InstanceValue::getInstanceSingleton() {
    return m_instance;
}

void InstanceValue::init() {
    m_instance.m_signature = &InstanceValue::getInstanceSingleton;
}

void InstanceValue::copy(const InstanceValue& rhs) {
    m_instance = rhs.m_instance;
}

InstanceValue::InstanceValue() : Element(ElementType::INSTANCE_VALUE) {
    init();
}

InstanceValue::InstanceValue(const InstanceValue& rhs) : Element(rhs, ElementType::INSTANCE_VALUE) {
    init();
    Element::copy(rhs);
    TypedElement::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    copy(rhs);
}

InstanceSpecification*  InstanceValue::getInstance() {
    return m_instance.get();
}

InstanceSpecification& InstanceValue::getInstanceRef() {
    return m_instance.getRef();
}

bool InstanceValue::hasInstance() const {
    return m_instance.has();
}

void InstanceValue::setInstance(InstanceSpecification* inst) {
    m_instance.set(inst);
}

void InstanceValue::setInstance(InstanceSpecification& inst) {
    m_instance.set(inst);
}

void InstanceValue::setInstance(ID id) {
    m_instance.set(id);
}

bool InstanceValue::isSubClassOf(ElementType eType) const {
    bool ret = ValueSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::INSTANCE_VALUE;
    }

    return ret;
}