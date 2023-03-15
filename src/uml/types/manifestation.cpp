#include "uml/types/manifestation.h"
#include "uml/types/profile.h"
#include "uml/types/artifact.h"
#include "uml/types/operation.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<PackageableElement, Manifestation>& Manifestation::getUtilizedElementSingleton() {
    return m_utilizedElement;
}

Manifestation::Manifestation() : Element(ElementType::MANIFESTATION) {
    m_utilizedElement.subsets(m_suppliers);
}

Manifestation::~Manifestation() {
    
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