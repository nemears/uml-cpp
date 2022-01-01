#include "uml/manifestation.h"
#include "uml/packageableElement.h"
#include "uml/artifact.h"
#include "uml/uml-stable.h"

using namespace UML;

void Manifestation::referencingReleased(ID id) {
    Abstraction::referencingReleased(id);
    m_utilizedElement.release(id);
}

void Manifestation::referenceReindexed(ID oldID, ID newID) {
    Abstraction::referenceReindexed(oldID, newID);
    m_utilizedElement.reindex(oldID, newID);
}

void Manifestation::restoreReferences() {
    Abstraction::restoreReferences();
    // m_utilizedElement.restoreReference();
}

void Manifestation::referenceErased(ID id) {
    Abstraction::referenceErased(id);
    m_utilizedElement.eraseElement(id);
}

Set<PackageableElement, Manifestation>& Manifestation::getUtilizedElementSingleton() {
    return m_utilizedElement;
}

void Manifestation::init() {
    m_utilizedElement.subsets(m_supplier);
    m_utilizedElement.m_signature = &Manifestation::getUtilizedElementSingleton;
}

void Manifestation::copy(const Manifestation& rhs) {
    m_utilizedElement = rhs.m_utilizedElement;
}

Manifestation::Manifestation() : Element(ElementType::MANIFESTATION) {
    init();
}

Manifestation::Manifestation(const Manifestation& rhs) : Element(rhs, ElementType::MANIFESTATION) {
    init();
    Element::copy(rhs);
    Relationship::copy(rhs);
    DirectedRelationship::copy(rhs);
    NamedElement::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    Dependency::copy(rhs);
    copy(rhs);
}

Manifestation::~Manifestation() {

}

PackageableElement* Manifestation::getUtilizedElement() {
    return m_utilizedElement.get();
}

PackageableElement& Manifestation::getUtilizedElementRef() {
    return m_utilizedElement.getRef();
}

bool Manifestation::hasUtilizedElement() const {
    return m_utilizedElement.has();
}

void Manifestation::setUtilizedElement(PackageableElement* utilizedElement) {
    m_utilizedElement.set(utilizedElement);
}

void Manifestation::setUtilizedElement(PackageableElement& utilizedElement) {
    m_utilizedElement.set(utilizedElement);
}

bool Manifestation::isSubClassOf(ElementType eType) const {
    bool ret = Abstraction::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MANIFESTATION;
    }

    return ret;
}