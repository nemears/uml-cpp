#include "uml/deploymentTarget.h"
#include "uml/deployment.h"
#include "uml/uml-stable.h"

using namespace UML;

void DeploymentTarget::init() {
    m_deployments.subsets(m_clientDependencies);
    m_deployments.subsets(*m_ownedElements);
    m_deployments.opposite(&Deployment::getLocationSingleton);
    m_deployments.m_signature = &DeploymentTarget::getDeployments;
}

void DeploymentTarget::copy(const DeploymentTarget& rhs) {
    m_deployments = rhs.m_deployments;
}

DeploymentTarget::DeploymentTarget() : Element(ElementType::DEPLOYMENT_TARGET) {
    init();
}

DeploymentTarget::DeploymentTarget(const DeploymentTarget& deploymentTarget) : Element(deploymentTarget, ElementType::DEPLOYMENT_TARGET) {
    // abstract
}

DeploymentTarget::~DeploymentTarget() {

}

Set<Deployment, DeploymentTarget>& DeploymentTarget::getDeployments() {
    return m_deployments;
}

bool DeploymentTarget::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DEPLOYMENT_TARGET;
    }

    return ret;
}