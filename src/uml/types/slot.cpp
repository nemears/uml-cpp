#include "uml/uml-stable.h"

using namespace UML;

void Slot::referenceErased(ID id) {
    Element::referenceErased(id);
    eraseFromSet(id, m_definingFeature);
}

Singleton<StructuralFeature, Slot>& Slot::getDefiningFeatureSingleton() {
    return m_definingFeature;
}

Singleton<InstanceSpecification, Slot>& Slot::getOwningInstanceSingleton() {
    return m_owningInstance;
}

Slot::Slot() : Element(ElementType::SLOT) {
    m_owningInstance.subsets(*m_owner);
    m_owningInstance.opposite(&InstanceSpecification::getSlots);
    m_values.subsets(*m_ownedElements);
}

Slot::~Slot() {
    
}

OrderedSet<ValueSpecification, Slot>& Slot::getValues() {
    return m_values;
}

StructuralFeaturePtr Slot::getDefiningFeature() const {
    return m_definingFeature.get();
}

void Slot::setDefiningFeature(StructuralFeature* definingFeature) {
    m_definingFeature.set(definingFeature);
}

void Slot::setDefiningFeature(StructuralFeature& definingFeature) {
    m_definingFeature.set(definingFeature);
}

void Slot::setDefiningFeature(ID id) {
    m_definingFeature.set(id);
}

InstanceSpecificationPtr Slot::getOwningInstance() const {
    return m_owningInstance.get();
}

void Slot::setOwningInstance(InstanceSpecification* inst) {
    m_owningInstance.set(inst);
}

void Slot::setOwningInstance(InstanceSpecification& inst) {
    m_owningInstance.set(inst);
}

void Slot::setOwningInstance(ID id) {
    m_owningInstance.set(id);
}

bool Slot::is(ElementType eType) const {
    bool ret = Element::is(eType);

    if(!ret) {
        ret = eType == ElementType::SLOT;
    }

    return ret;
}
