#include "uml/types/artifact.h"
#include "uml/types/property.h"
#include "uml/types/operation.h"
#include "uml/types/manifestation.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"

using namespace UML;

Artifact::Artifact() : Element(ElementType::ARTIFACT) {
    m_nestedArtifacts.subsets(m_ownedMembers);
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_manifestations.subsets(*m_ownedElements);
    m_manifestations.subsets(*m_clientDependencies);
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