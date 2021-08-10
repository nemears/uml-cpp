#include "uml/manifestation.h"
#include "uml/universalFunctions.h"
#include "uml/packageableElement.h"
#include "uml/artifact.h"

using namespace UML;

Manifestation::Manifestation() {
    m_utilizedElementPtr = 0;
    m_artifactPtr = 0;
}

Manifestation::Manifestation(const Manifestation& manifestation) {

}

Manifestation::~Manifestation() {

}

PackageableElement* Manifestation::getUtilizedElement() {
    return universalGet<PackageableElement>(m_utilizedElementID, m_utilizedElementPtr, m_manager);
}

void Manifestation::setUtilizedElement(PackageableElement* utilizedElement) {
    if (!isSameOrNull(m_utilizedElementID, utilizedElement)) {
        if (!m_utilizedElementPtr) {
            m_utilizedElementPtr = &m_manager->get<PackageableElement>(m_utilizedElementID);
        }

        if (m_supplier.count(m_utilizedElementID)) {
            m_supplier.remove(*m_utilizedElementPtr);
        }

        m_utilizedElementID = ID::nullID();
        m_utilizedElementPtr = 0;
    }

    if (utilizedElement) {
        m_utilizedElementID = utilizedElement->getID();
    }

    if (!m_manager) {
        m_utilizedElementPtr = utilizedElement;
    }

    if (utilizedElement) {
        if (!m_supplier.count(utilizedElement->getID())) {
            m_supplier.add(*utilizedElement);
        }
    }
}

Artifact* Manifestation::getArtifact() {
    return universalGet<Artifact>(m_artifactID, m_artifactPtr, m_manager);
}

void Manifestation::setArtifact(Artifact* artifact) {
    if (!isSameOrNull(m_artifactID, artifact)) {
        if (!m_artifactPtr) {
            m_artifactPtr = &m_manager->get<Artifact>(m_artifactID);
        }

        if (m_supplier.count(m_artifactID)) {
            m_supplier.remove(*m_artifactPtr);
        }

        if (m_artifactPtr->getManifestations().count(m_id)) {
            m_artifactPtr->getManifestations().remove(*this);
        }

        m_artifactID = ID::nullID();
        m_artifactPtr = 0;
    }

    if (artifact) {
        m_artifactID = artifact->getID();
    }

    if (!m_manager) {
        m_artifactPtr = artifact;
    }

    if (artifact) {
        if (!m_supplier.count(artifact->getID())) {
            m_supplier.add(*artifact);
        }

        if (!artifact->getManifestations().count(m_id)) {
            artifact->getManifestations().add(*this);
        }
    }
}

ElementType Manifestation::getElementType() const {
    return ElementType::MANIFESTATION;
}

bool Manifestation::isSubClassOf(ElementType eType) const {
    bool ret = Abstraction::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MANIFESTATION;
    }

    return ret;
}