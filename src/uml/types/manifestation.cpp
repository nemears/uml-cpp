#include "uml/set/singleton.h"
#include "uml/uml-stable.h"

using namespace UML;

Singleton<PackageableElement, Manifestation>& Manifestation::getUtilizedElementSingleton() {
    return m_utilizedElement;
}

void Manifestation::restoreReferences() {
    if (m_owner->get()) {
        if (m_owner->get()->is(ElementType::ARTIFACT)) {
            Artifact& possibleClient = m_owner->get()->as<Artifact>();
            if (possibleClient.getManifestations().contains(m_id) && !m_clients.contains(m_owner->get().id())) {
                m_clients.innerAdd(&possibleClient);
            }
        }
    }
}

Manifestation::Manifestation() : Element(ElementType::MANIFESTATION) {
    m_utilizedElement.subsets(m_suppliers);
}

Manifestation::~Manifestation() {
    
}

PackageableElementPtr Manifestation::getUtilizedElement() const {
    return m_utilizedElement.get();
}

void Manifestation::setUtilizedElement(PackageableElementPtr utilizedElement) {
    m_utilizedElement.set(utilizedElement);
}

void Manifestation::setUtilizedElement(PackageableElement& utilizedElement) {
    m_utilizedElement.set(utilizedElement);
}

void Manifestation::setUtilizedElement(ID id) {
    m_utilizedElement.set(id);
}

bool Manifestation::is(ElementType eType) const {
    bool ret = Abstraction::is(eType);

    if (!ret) {
        ret = eType == ElementType::MANIFESTATION;
    }

    return ret;
}
