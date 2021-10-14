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
    if (m_instance.id() == id) {
        m_instance.release();
    }
}

void InstanceValue::referenceReindexed(ID oldID, ID newID) {
    ValueSpecification::referenceReindexed(oldID, newID);
    if (m_instance.id() == oldID) {
        m_instance.reindex(oldID, newID);
    }
}

void InstanceValue::referenceErased(ID id) {
    ValueSpecification::referenceErased(id);
    m_instance.elementErased(id);
}

InstanceValue::InstanceValue() {
    m_instance.m_signature = &InstanceValue::m_instance;
    m_instance.m_addProcedures.push_back(new AddInstanceProcedure(this));
    m_instance.m_removeProcedures.push_back(new RemoveInstanceProcedure(this));
}

InstanceValue::InstanceValue(const InstanceValue& rhs) {
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