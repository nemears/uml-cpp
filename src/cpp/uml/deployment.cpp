#include "uml/deployment.h"
#include "uml/deployedArtifact.h"
#include "uml/deploymentTarget.h"
#include "uml/profile.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<DeploymentTarget, Deployment>& Deployment::getLocationSingleton() {
    return m_location;
}

Deployment::Deployment() : Element(ElementType::DEPLOYMENT) {
    m_location.subsets(m_clients);
    m_location.subsets(*m_owner);
    m_location.opposite(&DeploymentTarget::getDeployments);
    m_deployedArtifacts.subsets(m_suppliers);
}

Deployment::~Deployment() {
    
}

Set<DeployedArtifact, Deployment>& Deployment::getDeployedArtifacts() {
    return m_deployedArtifacts;
}

DeploymentTargetPtr Deployment::getLocation() const {
    return m_location.get();
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