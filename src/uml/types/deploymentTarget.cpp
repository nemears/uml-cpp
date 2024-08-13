#include "uml/uml-stable.h"

using namespace UML;

DeploymentTarget::DeploymentTarget() : Element(ElementType::DEPLOYMENT_TARGET) {
    m_deployments.subsets(m_clientDependencies);
    m_deployments.subsets(*m_ownedElements);
    m_deployments.opposite(&Deployment::getLocationSingleton);
}

DeploymentTarget::~DeploymentTarget() {

}

Set<Deployment, DeploymentTarget>& DeploymentTarget::getDeployments() {
    return m_deployments;
}

bool DeploymentTarget::is(ElementType eType) const {
    bool ret = NamedElement::is(eType);

    if (!ret) {
        ret = eType == ElementType::DEPLOYMENT_TARGET;
    }

    return ret;
}
