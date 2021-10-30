#include "uml/instanceValue.h"
#include "uml/instanceSpecification.h"

using namespace UML;

void InstanceValue::RemoveInstanceProcedure::operator()(InstanceSpecification* el) const {
    el->removeReference(m_me->getID());
}

void InstanceValue::AddInstanceProcedure::operator()(InstanceSpecification* el) const {
    el->setReference(m_me);
}

void InstanceValue::referencingReleased(ID id) {
    ValueSpecification::referencingReleased(id);
    m_instance.release(id);
}

void InstanceValue::referenceReindexed(ID oldID, ID newID) {
    ValueSpecification::referenceReindexed(oldID, newID);
    m_instance.reindex(oldID, newID);
}

void InstanceValue::restoreReferences() {
    ValueSpecification::restoreReferences();
    m_instance.restoreReference();
}

void InstanceValue::referenceErased(ID id) {
    ValueSpecification::referenceErased(id);
    m_instance.elementErased(id);
}

InstanceValue::InstanceValue() : Element(ElementType::INSTANCE_VALUE) {
    m_instance.m_signature = &InstanceValue::m_instance;
    m_instance.m_addProcedures.push_back(new AddInstanceProcedure(this));
    m_instance.m_removeProcedures.push_back(new RemoveInstanceProcedure(this));
}

InstanceValue::InstanceValue(const InstanceValue& rhs) : Element(rhs, ElementType::INSTANCE_VALUE) {
    m_instance = rhs.m_instance;
    m_instance.m_me = this;
    m_instance.m_addProcedures.push_back(new AddInstanceProcedure(this));
    m_instance.m_removeProcedures.push_back(new RemoveInstanceProcedure(this));
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

bool InstanceValue::isSubClassOf(ElementType eType) const {
    bool ret = ValueSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::INSTANCE_VALUE;
    }

    return ret;
}