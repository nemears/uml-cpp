#include "uml/instanceValue.h"
#include "uml/instanceSpecification.h"
#include "uml/umlPtr.h"
#include "uml/behavior.h"
#include "uml/association.h"
#include "uml/dataType.h"
#include "uml/stereotype.h"
#include "uml/deployment.h"

using namespace UML;

void InstanceValue::referencingReleased(ID id) {
    ValueSpecification::referencingReleased(id);
    m_instance.release(id);
}

void InstanceValue::referenceReindexed(ID oldID, ID newID) {
    ValueSpecification::referenceReindexed(oldID, newID);
    m_instance.reindex(oldID, newID);
}

void InstanceValue::reindexName(std::string oldName, std::string newName) {
    ValueSpecification::reindexName(oldName, newName);
    m_instance.reindexName(oldName, newName);
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

InstanceValue::InstanceValue() : Element(ElementType::INSTANCE_VALUE) {
    init();
}

InstanceValue::~InstanceValue() {
    mountAndRelease();
}

InstanceSpecificationPtr  InstanceValue::getInstance() const {
    return m_instance.get();
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