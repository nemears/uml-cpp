#include "uml/uml-stable.h"

using namespace UML;

Artifact::Artifact() : Element(ElementType::ARTIFACT) {
    m_nestedArtifacts.subsets(m_ownedMembers);
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_manifestations.subsets(*m_ownedElements);
    m_manifestations.subsets(m_clientDependencies);
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


bool Artifact::is(ElementType eType) const {
    bool ret = Classifier::is(eType);

    if (!ret) {
        ret = DeployedArtifact::is(eType);
    }

    if (!ret) {
        ret = eType == ElementType::ARTIFACT;
    }

    return ret;
}
