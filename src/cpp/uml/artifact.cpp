#include "uml/artifact.h"
#include "uml/property.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"

using namespace UML;

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
    m_manifestations.subsets(*m_clientDependencies);
    m_manifestations.m_signature = &Artifact::getManifestations;
}

Artifact::Artifact() : Element(ElementType::ARTIFACT) {
    init();
}

Artifact::~Artifact() {
    mountAndRelease();
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