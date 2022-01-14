#include "uml/deployment.h"
#include "uml/deployedArtifact.h"
#include "uml/deploymentTarget.h"
#include "uml/uml-stable.h"

using namespace UML;

void Deployment::referencingReleased(ID id) {
    Dependency::referencingReleased(id);
    m_location.release(id);
    m_deployedArtifacts.release(id);
}

void Deployment::referenceReindexed(ID oldID, ID newID) {
    Dependency::referenceReindexed(oldID, newID);
    m_location.reindex(oldID, newID);
    m_deployedArtifacts.reindex(oldID, newID);
}

void Deployment::restoreReferences() {
    Dependency::restoreReferences();
    // m_location.restoreReference();
    // m_deployedArtifacts.restoreReferences();
}

void Deployment::referenceErased(ID id) {
    Dependency::referenceErased(id);
    m_location.eraseElement(id);
    m_deployedArtifacts.eraseElement(id);
}

Set<DeploymentTarget, Deployment>& Deployment::getLocationSingleton() {
    return m_location;
}

void Deployment::init() {
    m_location.subsets(m_client);
    m_location.subsets(*m_owner);
    m_location.opposite(&DeploymentTarget::getDeployments);
    m_location.m_signature = &Deployment::getLocationSingleton;
    m_deployedArtifacts.subsets(m_supplier);
    m_deployedArtifacts.m_signature = &Deployment::getDeployedArtifacts;
}

void Deployment::copy(const Deployment& rhs) {
    m_location = rhs.m_location;
    m_deployedArtifacts = rhs.m_deployedArtifacts;
}

Deployment::Deployment() : Element(ElementType::DEPLOYMENT) {
    init();
}

Deployment::Deployment(const Deployment& rhs) : Element(ElementType::DEPLOYMENT) {
    init();
    Relationship::copy(rhs);
    DirectedRelationship::copy(rhs);
    NamedElement::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    Dependency::copy(rhs);
    copy(rhs);
}

Deployment::~Deployment() {

}

Set<DeployedArtifact, Deployment>& Deployment::getDeployedArtifacts() {
    return m_deployedArtifacts;
}

DeploymentTarget* Deployment::getLocation() {
    return m_location.get();
}

DeploymentTarget& Deployment::getLocationRef() {
    return m_location.getRef();
}

ID Deployment::getLocationID() const {
    return m_location.id();
}

bool Deployment::hasLocation() const {
    return m_location.has();
}

void Deployment::setLocation(DeploymentTarget* location) {
    m_location.set(location);
}

void Deployment::setLocation(DeploymentTarget& location) {
    m_location.set(location);
}

void Deployment::setLocation(ID id) {
    m_location.set(id);
}

bool Deployment::isSubClassOf(ElementType eType) const {
    bool ret = Dependency::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DEPLOYMENT;
    }

    return ret;
}