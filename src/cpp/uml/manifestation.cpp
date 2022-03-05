#include "uml/manifestation.h"
#include "uml/profile.h"
#include "uml/artifact.h"
#include "uml/operation.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

Set<PackageableElement, Manifestation>& Manifestation::getUtilizedElementSingleton() {
    return m_utilizedElement;
}

void Manifestation::init() {
    m_utilizedElement.subsets(m_supplier);
    m_utilizedElement.m_signature = &Manifestation::getUtilizedElementSingleton;
}

Manifestation::Manifestation() : Element(ElementType::MANIFESTATION) {
    init();
}

Manifestation::~Manifestation() {
    mountAndRelease();
}

PackageableElementPtr Manifestation::getUtilizedElement() const {
    return m_utilizedElement.get();
}

void Manifestation::setUtilizedElement(PackageableElement* utilizedElement) {
    m_utilizedElement.set(utilizedElement);
}

void Manifestation::setUtilizedElement(PackageableElement& utilizedElement) {
    m_utilizedElement.set(utilizedElement);
}

void Manifestation::setUtilizedElement(ID id) {
    m_utilizedElement.set(id);
}

bool Manifestation::isSubClassOf(ElementType eType) const {
    bool ret = Abstraction::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MANIFESTATION;
    }

    return ret;
}