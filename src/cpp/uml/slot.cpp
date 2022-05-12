#include "uml/slot.h"
#include "uml/instanceSpecification.h"
#include "uml/structuralFeature.h"
#include "uml/valueSpecification.h"
#include "uml/behavior.h"
#include "uml/property.h"
#include "uml/dataType.h"
#include "uml/generalization.h"
#include "uml/package.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"
#include "uml/setReferenceFunctor.h"

using namespace UML;

void Slot::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_definingFeature.release(id);
}

void Slot::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_definingFeature.reindex(oldID, newID);
}

void Slot::referenceErased(ID id) {
    Element::referenceErased(id);
    m_definingFeature.eraseElement(id);
}

void Slot::restoreReference(Element* el) {
    Element::restoreReference(el);
    m_definingFeature.restore(el);
}

Set<StructuralFeature, Slot>& Slot::getDefiningFeatureSingleton() {
    return m_definingFeature;
}

Set<InstanceSpecification, Slot>& Slot::getOwningInstanceSingleton() {
    return m_owningInstance;
}

void Slot::init() {
    m_definingFeature.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_definingFeature.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_owningInstance.subsets(*m_owner);
    m_owningInstance.opposite(&InstanceSpecification::getSlots);
    m_values.subsets(*m_ownedElements);
}

Slot::Slot() : Element(ElementType::SLOT) {
    init();
}

Slot::~Slot() {
    mountAndRelease();
}

Set<ValueSpecification, Slot>& Slot::getValues() {
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