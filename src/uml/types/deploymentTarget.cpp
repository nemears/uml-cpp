#include "uml/uml-stable.h"

using namespace UML;

DeploymentTarget::DeploymentTarget(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager)
{
    m_deployments.subsets(m_clientDependencies);
    m_deployments.subsets(m_ownedElements);
    m_deployments.opposite(&Deployment::getLocationSingleton);
}

Set<Deployment, DeploymentTarget>& DeploymentTarget::getDeployments() {
    return m_deployments;
}
