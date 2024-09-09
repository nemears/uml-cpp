#include "uml/uml-stable.h"

using namespace UML;

Singleton<DeploymentTarget, Deployment>& Deployment::getLocationSingleton() {
    return m_location;
}

Deployment::Deployment(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    Dependency(elementType, manager)
{
    m_location.subsets(m_clients);
    m_location.subsets(m_owner);
    m_location.opposite(&DeploymentTarget::getDeployments);
    m_deployedArtifacts.subsets(m_suppliers);
}

Set<DeployedArtifact, Deployment>& Deployment::getDeployedArtifacts() {
    return m_deployedArtifacts;
}

DeploymentTargetPtr Deployment::getLocation() const {
    return m_location.get();
}

void Deployment::setLocation(DeploymentTargetPtr location) {
    m_location.set(location);
}

void Deployment::setLocation(DeploymentTarget& location) {
    m_location.set(location);
}

void Deployment::setLocation(ID id) {
    m_location.set(id);
}
