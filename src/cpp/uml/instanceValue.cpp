#include "uml/instanceValue.h"
#include "uml/instanceSpecification.h"
#include "uml/umlPtr.h"
#include "uml/behavior.h"
#include "uml/association.h"
#include "uml/dataType.h"
#include "uml/stereotype.h"
#include "uml/deployment.h"

using namespace UML;

void InstanceValue::referenceReindexed(ID newID) {
    ValueSpecification::referenceReindexed(newID);
    m_instance.reindex(newID);
}

void InstanceValue::referenceErased(ID id) {
    ValueSpecification::referenceErased(id);
    m_instance.eraseElement(id);
}

void InstanceValue::restoreReference(Element* el) {
    ValueSpecification::restoreReference(el);
    // m_instance.restore(el);
}

TypedSet<InstanceSpecification, InstanceValue>& InstanceValue::getInstanceSingleton() {
    return m_instance;
}

void InstanceValue::init() {
    
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