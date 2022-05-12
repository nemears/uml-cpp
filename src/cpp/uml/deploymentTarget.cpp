#include "uml/deploymentTarget.h"
#include "uml/deployment.h"
#include "uml/profile.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"

using namespace UML;

void DeploymentTarget::init() {
    m_deployments.subsets(*m_clientDependencies);
    m_deployments.subsets(*m_ownedElements);
    m_deployments.opposite(&Deployment::getLocationSingleton);
}

DeploymentTarget::DeploymentTarget() : Element(ElementType::DEPLOYMENT_TARGET) {
    init();
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