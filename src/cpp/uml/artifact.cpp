#include "uml/artifact.h"
#include "uml/property.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/uml-stable.h"

using namespace UML;

void Artifact::referenceReindexed(ID oldID, ID newID) {
    Classifier::referenceReindexed(oldID, newID);
    DeployedArtifact::referenceReindexed(oldID, newID);
    m_ownedAttributes.reindex(oldID, newID);
    m_ownedOperations.reindex(oldID, newID);
    m_nestedArtifacts.reindex(oldID, newID);
    m_manifestations.reindex(oldID, newID);
}

void Artifact::referencingReleased(ID id) {
    Classifier::referencingReleased(id);
    DeployedArtifact::referencingReleased(id);
    m_ownedAttributes.release(id);
    m_ownedOperations.release(id);
    m_nestedArtifacts.release(id);
    m_manifestations.release(id);
}

void Artifact::referenceErased(ID id) {
    Classifier::referenceErased(id);
    m_ownedAttributes.eraseElement(id);
    m_ownedOperations.eraseElement(id);
    m_nestedArtifacts.eraseElement(id);
    m_manifestations.eraseElement(id);
}

Set<Property, Artifact>& Artifact::getOwnedAttributesSet() {
    return m_ownedAttributes;
}

Set<Operation, Artifact>& Artifact::getOwnedOperationsSet() {
    return m_ownedOperations;
}

void Artifact::init() {
    m_nestedArtifacts.subsets(m_ownedMembers);
    m_nestedArtifacts.m_signature = &Artifact::getNestedArtifacts;
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.m_signature = &Artifact::getOwnedAttributesSet;
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_ownedOperations.m_signature = &Artifact::getOwnedOperationsSet;
    m_manifestations.subsets(m_ownedMembers);
    m_manifestations.subsets(m_clientDependencies);
    m_manifestations.m_signature = &Artifact::getManifestations;
}

void Artifact::copy(const Artifact& rhs) {
    m_nestedArtifacts = rhs.m_nestedArtifacts;
    m_ownedAttributes = rhs.m_ownedAttributes;
    m_ownedOperations = rhs.m_ownedOperations;
    m_manifestations = rhs.m_manifestations;
}

Artifact::Artifact() : Element(ElementType::ARTIFACT) {
    init();
}

Artifact::Artifact(const Artifact& rhs) : Element(rhs, ElementType::ARTIFACT) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    TemplateableElement::copy(rhs);
    RedefinableElement::copy(rhs);
    Classifier::copy(rhs);
    DeployedArtifact::copy(rhs);
    copy(rhs);
}

Artifact::~Artifact() {
    
}

OrderedSet<Property, Artifact>& Artifact::getOwnedAttributes() {
    return m_ownedAttributes;
}

OrderedSet<Operation, Artifact>& Artifact::getOwnedOperations() {
    return m_ownedOperations;
}

Set<Artifact, Artifact>& Artifact::getNestedArtifacts() {
    return m_nestedArtifacts;
}

Set<Manifestation, Artifact>& Artifact::getManifestations() {
    return m_manifestations;
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