#include "uml/manifestation.h"
#include "uml/packageableElement.h"
#include "uml/artifact.h"

using namespace UML;

void Manifestation::RemoveUtilizedElementProcedure::operator()(PackageableElement* el) const {
    if (m_me->getSupplier().count(el->getID())) {
        m_me->getSupplier().remove(*el);
    }
}

void Manifestation::AddUtilizedElementProcedure::operator()(PackageableElement* el) const {
    if (!m_me->getSupplier().count(el->getID())) {
        m_me->getSupplier().add(*el);
    }
}

void Manifestation::RemoveArtifactProcedure::operator()(Artifact* el) const {
    if (m_me->getClient().count(el->getID())) {
        m_me->getClient().remove(*el);
    }
    if (el->getManifestations().count(m_me->getID())) {
        el->getManifestations().remove(*m_me);
    }
}

void Manifestation::AddArtifactProcedure::operator()(Artifact* el) const {
    if (!m_me->getClient().count(el->getID())) {
        m_me->getClient().add(*el);
    }
    if (!el->getManifestations().count(m_me->getID())) {
        el->getManifestations().add(*m_me);
    }
    if (m_me->getOwnerID() != el->getID()) {
        m_me->setOwner(el);
    }
}

void Manifestation::AddArtifactProcedure::operator()(ID id) const {
    if (!m_me->getClient().count(id)) {
        m_me->getClient().addByID(id);
    }

    if (m_me->getOwnerID() != id) {
        m_me->setOwnerByID(id);
    }
}

void Manifestation::referencingReleased(ID id) {
    Abstraction::referencingReleased(id);
    if (m_utilizedElement.id() == id) {
        m_utilizedElement.release();
    }
    if (m_artifact.id() == id) {
        m_utilizedElement.release();
    }
}

void Manifestation::referenceReindexed(ID oldID, ID newID) {
    Abstraction::referenceReindexed(oldID, newID);
    if (m_utilizedElement.id() == oldID) {
        m_utilizedElement.reindex(oldID, newID);
    }
    if (m_artifact.id() == oldID) {
        m_artifact.reindex(oldID, newID);
    }
}

void Manifestation::restoreReferences() {
    Abstraction::restoreReferences();
    m_utilizedElement.restoreReference();
    m_artifact.restoreReference();
}

Manifestation::Manifestation() {
    m_utilizedElement.m_signature = &Manifestation::m_utilizedElement;
    m_utilizedElement.m_addProcedures.push_back(new AddUtilizedElementProcedure(this));
    m_utilizedElement.m_removeProcedures.push_back(new RemoveUtilizedElementProcedure(this));
    m_artifact.m_signature = &Manifestation::m_artifact;
    m_artifact.m_addProcedures.push_back(new AddArtifactProcedure(this));
    m_artifact.m_removeProcedures.push_back(new RemoveArtifactProcedure(this));
}

Manifestation::Manifestation(const Manifestation& manifestation) {
    m_utilizedElement = manifestation.m_utilizedElement;
    m_utilizedElement.m_me = this;
    m_utilizedElement.m_addProcedures.clear();
    m_utilizedElement.m_removeProcedures.clear();
    m_utilizedElement.m_addProcedures.push_back(new AddUtilizedElementProcedure(this));
    m_utilizedElement.m_removeProcedures.push_back(new RemoveUtilizedElementProcedure(this));
    m_artifact = manifestation.m_artifact;
    m_artifact.m_me = this;
    m_artifact.m_addProcedures.clear();
    m_artifact.m_removeProcedures.clear();
    m_artifact.m_addProcedures.push_back(new AddArtifactProcedure(this));
    m_artifact.m_removeProcedures.push_back(new RemoveArtifactProcedure(this));
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

Artifact* Manifestation::getArtifact() {
    return m_artifact.get();
}

Artifact& Manifestation::getArtifactRef() {
    return m_artifact.getRef();
}

ID Manifestation::getArtifactID() const {
    return m_artifact.id();
}

bool Manifestation::hasArtifact() const {
    return m_artifact.has();
}

void Manifestation::setArtifact(Artifact* artifact) {
    m_artifact.set(artifact);
}

void Manifestation::setArtifact(Artifact& artifact) {
    m_artifact.set(artifact);
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