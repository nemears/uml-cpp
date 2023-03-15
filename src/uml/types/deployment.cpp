#include "uml/types/deployment.h"
#include "uml/types/deployedArtifact.h"
#include "uml/types/deploymentTarget.h"
#include "uml/types/profile.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
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