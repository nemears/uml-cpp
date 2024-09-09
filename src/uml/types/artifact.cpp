#include "uml/uml-stable.h"

using namespace UML;

Artifact::Artifact(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Classifier(elementType, manager),
    DeployedArtifact(elementType, manager)
{
    m_nestedArtifacts.subsets(m_ownedMembers);
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_manifestations.subsets(m_ownedElements);
    m_manifestations.subsets(m_clientDependencies);
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
