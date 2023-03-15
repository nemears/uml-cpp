#include "uml/types/deploymentTarget.h"
#include "uml/types/deployment.h"
#include "uml/types/profile.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"

using namespace UML;

DeploymentTarget::DeploymentTarget() : Element(ElementType::DEPLOYMENT_TARGET) {
    m_deployments.subsets(*m_clientDependencies);
    m_deployments.subsets(*m_ownedElements);
    m_deployments.opposite(&Deployment::getLocationSingleton);
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