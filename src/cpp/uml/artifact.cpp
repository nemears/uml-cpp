#include "uml/artifact.h"
#include "uml/property.h"
#include "uml/operation.h"
#include "uml/manifestation.h"

using namespace UML;

void Artifact::setManager(UmlManager* manager) {
    Classifier::setManager(manager);
    DeployedArtifact::setManager(manager);
    m_ownedAttributes.m_manager = manager;
    m_ownedOperations.m_manager = manager;
    m_nestedArtifacts.m_manager = manager;
    m_manifestations.m_manager = manager;
}

void Artifact::AddOwnedAttributeFunctor::operator()(Property& el) const {
    if (!m_el->getAttributes().count(el.getID())) {
        m_el->getAttributes().add(el);
    }

    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }

    if (el.getArtifact() != m_el) {
        el.setArtifact(m_el);
    }

    m_el->getOwnedAttributes().updateCopiedSequenceAddedTo<Artifact>(el, &Artifact::getOwnedAttributes);
}

void Artifact::RemoveOwnedAttributeFunctor::operator()(Property& el) const {
    if (m_el->getAttributes().count(el.getID())) {
        m_el->getAttributes().remove(el);
    }

    if (m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().remove(el);
    }

    if (el.getArtifact() == m_el) {
        el.setArtifact(0);
    }

    m_el->getOwnedAttributes().updateCopiedSequenceRemovedFrom<Artifact>(el, &Artifact::getOwnedAttributes);
}

void Artifact::AddOwnedOperationFunctor::operator()(Operation& el) const {
    if (!m_el->getFeatures().count(el.getID())) {
        m_el->getFeatures().add(el);
    }

    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }

    if (el.getArtifact() != m_el) {
        el.setArtifact(m_el);
    }

    if (!el.getRedefinitionContext().count(m_el->getID())) {
        el.getRedefinitionContext().add(*m_el);
    }

    m_el->getOwnedOperations().updateCopiedSequenceAddedTo<Artifact>(el, &Artifact::getOwnedOperations);
}

void Artifact::RemoveOwnedOperationFunctor::operator()(Operation& el) const {
    if (m_el->getFeatures().count(el.getID())) {
        m_el->getFeatures().remove(el);
    }

    if (m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().remove(el);
    }

    if (el.getArtifact() == m_el) {
        el.setArtifact(0);
    }

    if (el.getRedefinitionContext().count(m_el->getID())) {
        el.getRedefinitionContext().remove(*m_el);
    }

    m_el->getOwnedOperations().updateCopiedSequenceRemovedFrom<Artifact>(el, &Artifact::getOwnedOperations);
}

void Artifact::CheckNestedArtifactFunctor::operator()(Artifact& el) const {
    if (el.getID() == m_el->getID()) {
        throw NestedArtifactException();
    }
}

void Artifact::AddNestedArtifactFunctor::operator()(Artifact& el) const {
    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }

    m_el->getNestedArtifacts().updateCopiedSequenceAddedTo<Artifact>(el, &Artifact::getNestedArtifacts);
}

void Artifact::RemoveNestedArtifactFunctor::operator()(Artifact& el) const {
    if (m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().remove(el);
    }

    m_el->getNestedArtifacts().updateCopiedSequenceRemovedFrom<Artifact>(el, &Artifact::getNestedArtifacts);
}

void Artifact::AddManifestationFunctor::operator()(Manifestation& el) const {
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }

    el.setArtifact(m_el);
    m_el->getManifestations().updateCopiedSequenceAddedTo<Artifact>(el, &Artifact::getManifestations);
}

void Artifact::RemoveManifestationFunctor::operator()(Manifestation& el) const {
    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }

    el.setArtifact(0); 
    m_el->getManifestations().updateCopiedSequenceRemovedFrom<Artifact>(el, &Artifact::getManifestations);
}

Artifact::Artifact() {
    m_ownedAttributes.addProcedures.push_back(new AddOwnedAttributeFunctor(this));
    m_ownedAttributes.removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
    m_ownedOperations.addProcedures.push_back(new AddOwnedOperationFunctor(this));
    m_ownedOperations.removeProcedures.push_back(new RemoveOwnedOperationFunctor(this));
    m_nestedArtifacts.addProcedures.push_back(new AddNestedArtifactFunctor(this));
    m_nestedArtifacts.removeProcedures.push_back(new RemoveNestedArtifactFunctor(this));
    m_nestedArtifacts.addChecks.push_back(new CheckNestedArtifactFunctor(this));
    m_manifestations.addProcedures.push_back(new AddManifestationFunctor(this));
    m_manifestations.removeProcedures.push_back(new RemoveManifestationFunctor(this));
}

Artifact::Artifact(const Artifact& artifact) : DeployedArtifact(artifact),
Classifier(artifact),
PackageableElement(artifact), 
NamedElement(artifact),
Element(artifact) {
    m_ownedAttributes = artifact.m_ownedAttributes;
    m_ownedAttributes.m_el = this;
    m_ownedAttributes.addProcedures.clear();
    m_ownedAttributes.addProcedures.push_back(new AddOwnedAttributeFunctor(this));
    m_ownedAttributes.removeProcedures.clear();
    m_ownedAttributes.removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
    m_ownedOperations = artifact.m_ownedOperations;
    m_ownedOperations.m_el = this;
    m_ownedOperations.addProcedures.clear();
    m_ownedOperations.addProcedures.push_back(new AddOwnedOperationFunctor(this));
    m_ownedOperations.removeProcedures.clear();
    m_ownedOperations.removeProcedures.push_back(new RemoveOwnedOperationFunctor(this));
    m_nestedArtifacts = artifact.m_nestedArtifacts;
    m_nestedArtifacts.m_el = this;
    m_nestedArtifacts.addProcedures.clear();
    m_nestedArtifacts.removeProcedures.clear();
    m_nestedArtifacts.addProcedures.push_back(new AddNestedArtifactFunctor(this));
    m_nestedArtifacts.removeProcedures.push_back(new RemoveNestedArtifactFunctor(this));
    m_nestedArtifacts.addChecks.clear();
    m_nestedArtifacts.addChecks.push_back(new CheckNestedArtifactFunctor(this));
    m_manifestations = artifact.m_manifestations;
    m_manifestations.m_el = this;
    m_manifestations.addProcedures.clear();
    m_manifestations.removeProcedures.clear();
    m_manifestations.addProcedures.push_back(new AddManifestationFunctor(this));
    m_manifestations.removeProcedures.push_back(new RemoveManifestationFunctor(this));
}

Sequence<Property>& Artifact::getOwnedAttributes() {
    return m_ownedAttributes;
}

Sequence<Operation>& Artifact::getOwnedOperations() {
    return m_ownedOperations;
}

Sequence<Artifact>& Artifact::getNestedArtifacts() {
    return m_nestedArtifacts;
}

Sequence<Manifestation>& Artifact::getManifestations() {
    return m_manifestations;
}

Artifact::~Artifact() {

}

ElementType Artifact::getElementType() const {
    return ElementType::ARTIFACT;
}

bool Artifact::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = DeployedArtifact::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::ARTIFACT;
    }

    return ret;
}