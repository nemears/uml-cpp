#include "uml/types/slot.h"
#include "uml/types/instanceSpecification.h"
#include "uml/types/structuralFeature.h"
#include "uml/types/valueSpecification.h"
#include "uml/types/behavior.h"
#include "uml/types/property.h"
#include "uml/types/dataType.h"
#include "uml/types/generalization.h"
#include "uml/types/package.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Slot::referenceReindexed(ID newID) {
    Element::referenceReindexed(newID);
    m_definingFeature.reindex(newID);
}

void Slot::referenceErased(ID id) {
    Element::referenceErased(id);
    m_definingFeature.eraseElement(id);
}

void Slot::restoreReference(Element* el) {
    Element::restoreReference(el);
    // m_definingFeature.restore(el);
}

TypedSet<StructuralFeature, Slot>& Slot::getDefiningFeatureSingleton() {
    return m_definingFeature;
}

TypedSet<InstanceSpecification, Slot>& Slot::getOwningInstanceSingleton() {
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

bool Slot::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if(!ret) {
        ret = eType == ElementType::SLOT;
    }

    return ret;
}