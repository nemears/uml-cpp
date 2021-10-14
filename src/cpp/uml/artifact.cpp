#include "uml/artifact.h"
#include "uml/property.h"
#include "uml/operation.h"
#include "uml/manifestation.h"

using namespace UML;

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

    updateCopiedSequenceAddedTo(el, &Artifact::getOwnedAttributes);
}

void Artifact::AddOwnedAttributeFunctor::operator()(ID id) const {
    if (!m_el->getAttributes().count(id)) {
        m_el->getAttributes().addByID(id);
    }
    if (!m_el->getOwnedMembers().count(id)) {
        m_el->getOwnedMembers().addByID(id);
    }
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

    updateCopiedSequenceRemovedFrom(el, &Artifact::getOwnedAttributes);
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

    updateCopiedSequenceAddedTo(el, &Artifact::getOwnedOperations);
}

void Artifact::AddOwnedOperationFunctor::operator()(ID id) const {
    if (!m_el->getFeatures().count(id)) {
        m_el->getFeatures().addByID(id);
    }
    if (!m_el->getOwnedMembers().count(id)) {
        m_el->getOwnedMembers().addByID(id);
    }
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

    updateCopiedSequenceRemovedFrom(el, &Artifact::getOwnedOperations);
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

    if (el.getArtifactID() != m_el->getID()) {
        el.setArtifact(m_el);
    }

    updateCopiedSequenceAddedTo(el, &Artifact::getNestedArtifacts);
}

void Artifact::AddNestedArtifactFunctor::operator()(ID id) const {
    if (!m_el->getOwnedMembers().count(id)) {
        m_el->getOwnedMembers().addByID(id);
    }
}

void Artifact::RemoveNestedArtifactFunctor::operator()(Artifact& el) const {
    if (m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().remove(el);
    }

    if (el.getArtifactID() == m_el->getID()) {
        el.setArtifact(0);
    }

    updateCopiedSequenceRemovedFrom(el, &Artifact::getNestedArtifacts);
}

void Artifact::AddManifestationFunctor::operator()(Manifestation& el) const {
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }

    if (!m_el->getDirectedRelationships().count(el.getID())) {
        m_el->getDirectedRelationships().add(el);
    }

    el.setArtifact(m_el);
    updateCopiedSequenceAddedTo(el, &Artifact::getManifestations);
}

void Artifact::AddManifestationFunctor::operator()(ID id) const {
    if (!m_el->getOwnedElements().count(id)) {
        m_el->getOwnedElements().addByID(id);
    }
    if (!m_el->getDirectedRelationships().count(id)) {
        m_el->getDirectedRelationships().addByID(id);
    }
}

void Artifact::RemoveManifestationFunctor::operator()(Manifestation& el) const {
    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }

    if (m_el->getDirectedRelationships().count(el.getID())) {
        m_el->getDirectedRelationships().remove(el);
    }

    el.setArtifact(0); 
    updateCopiedSequenceRemovedFrom(el, &Artifact::getManifestations);
}

void Artifact::AddArtifactProcedure::operator()(Artifact* el) const {
    if (!el->getNestedArtifacts().count(m_me->getID())) {
        el->getNestedArtifacts().add(*m_me);
    }
    if (m_me->getNamespaceID() != el->getID()) {
        m_me->setNamespace(el);
    }
}

void Artifact::AddArtifactProcedure::operator()(ID id) const {
    if (m_me->getNamespaceID() != id) {
        m_me->m_namespace.setByID(id);
    }
}

void Artifact::RemoveArtifactProcedure::operator()(Artifact* el) const {
    if (el->getNestedArtifacts().count(m_me->getID())) {
        el->getNestedArtifacts().remove(*m_me);
    }
    if (m_me->getNamespaceID() == el->getID()) {
        m_me->setNamespace(0);
    }
}

void Artifact::setManager(UmlManager* manager) {
    Classifier::setManager(manager);
    DeployedArtifact::setManager(manager);
    m_ownedAttributes.m_manager = manager;
    m_ownedOperations.m_manager = manager;
    m_nestedArtifacts.m_manager = manager;
    m_manifestations.m_manager = manager;
}

void Artifact::referenceReindexed(ID oldID, ID newID) {
    Classifier::referenceReindexed(oldID, newID);
    DeployedArtifact::referenceReindexed(oldID, newID);
    if (m_ownedAttributes.count(oldID)) {
        m_ownedAttributes.reindex(oldID, newID, &Artifact::getOwnedAttributes);
    }
    if (m_ownedOperations.count(oldID)) {
        m_ownedOperations.reindex(oldID, newID, &Artifact::getOwnedOperations);
    }
    if (m_nestedArtifacts.count(oldID)) {
        m_nestedArtifacts.reindex(oldID, newID, &Artifact::getNestedArtifacts);
    }
    if (m_manifestations.count(oldID)) {
        m_manifestations.reindex(oldID, newID, &Artifact::getManifestations);
    }
    if (m_artifact.id() == oldID) {
        m_artifact.reindex(oldID, newID);
    }
}

void Artifact::referencingReleased(ID id) {
    Classifier::referencingReleased(id);
    DeployedArtifact::referencingReleased(id);
    if (m_ownedAttributes.count(id)) {
        m_ownedAttributes.elementReleased(id, &Artifact::getOwnedAttributes);
    }
    if (m_ownedOperations.count(id)) {
        m_ownedOperations.elementReleased(id, &Artifact::getOwnedOperations);
    }
    if (m_nestedArtifacts.count(id)) {
        m_nestedArtifacts.elementReleased(id, &Artifact::getNestedArtifacts);
    }
    if (m_manifestations.count(id)) {
        m_manifestations.elementReleased(id, &Artifact::getManifestations);
    }
    if (m_artifact.id() == id) {
        m_artifact.release();
    }
}

void Artifact::restoreReferences() {
    Classifier::restoreReferences();
    DeployedArtifact::restoreReferences();
    m_ownedAttributes.restoreReferences();
    m_ownedOperations.restoreReferences();
    m_nestedArtifacts.restoreReferences();
    m_manifestations.restoreReferences();
    m_artifact.restoreReference();
}

void Artifact::referenceErased(ID id) {
    Classifier::referenceErased(id);
    m_ownedAttributes.elementErased(id);
    m_ownedOperations.elementErased(id);
    m_nestedArtifacts.elementErased(id);
    m_manifestations.elementErased(id);
    m_artifact.elementErased(id);
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
    m_artifact.m_signature = &Artifact::m_artifact;
    m_artifact.m_removeProcedures.push_back(new RemoveArtifactProcedure(this));
    m_artifact.m_addProcedures.push_back(new AddArtifactProcedure(this));
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
    m_artifact = artifact.m_artifact;
    m_artifact.m_me = this;
    m_artifact.m_addProcedures.clear();
    m_artifact.m_removeProcedures.clear();
    m_artifact.m_removeProcedures.push_back(new RemoveArtifactProcedure(this));
    m_artifact.m_addProcedures.push_back(new AddArtifactProcedure(this));
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

Artifact* Artifact::getArtifact() {
    return m_artifact.get();
}

Artifact& Artifact::getArtifactRef() {
    return m_artifact.getRef();
}

ID Artifact::getArtifactID() const {
    return m_artifact.id();
}

bool Artifact::hasArtifact() const {
    return m_artifact.has();
}

void Artifact::setArtifact(Artifact* artifact) {
    m_artifact.set(artifact);
}

void Artifact::setArtifact(Artifact& artifact) {
    m_artifact.set(artifact);
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