#include "uml/types/instanceValue.h"
#include "uml/types/instanceSpecification.h"
#include "uml/umlPtr.h"
#include "uml/types/behavior.h"
#include "uml/types/association.h"
#include "uml/types/dataType.h"
#include "uml/types/stereotype.h"
#include "uml/types/deployment.h"

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

InstanceValue::InstanceValue() : Element(ElementType::INSTANCE_VALUE) {
    
}

InstanceValue::~InstanceValue() {
    
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