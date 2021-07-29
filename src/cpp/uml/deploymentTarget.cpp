#include "uml/deploymentTarget.h"

using namespace UML;

DeploymentTarget::DeploymentTarget() {

}

DeploymentTarget::DeploymentTarget(const DeploymentTarget& deploymentTarget) {

}

DeploymentTarget::~DeploymentTarget() {

}

ElementType DeploymentTarget::getElementType() const {
    return ElementType::DEPLOYMENT_TARGET;
}

bool DeploymentTarget::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DEPLOYMENT_TARGET;
    }

    return ret;
}