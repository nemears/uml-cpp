#include "uml/deploymentTarget.h"
#include "uml/deployment.h"

using namespace UML;

void DeploymentTarget::AddDeploymentFunctor::operator()(Element& el) const {
    if (!m_el->as<DeploymentTarget>().getOwnedElements().count(el.getID())) {
        m_el->as<DeploymentTarget>().getOwnedElements().internalAdd(el);
    }

    el.as<Deployment>().setLocation(&m_el->as<DeploymentTarget>());
}

void DeploymentTarget::RemoveDeploymentFunctor::operator()(Element& el) const {
    if (m_el->as<DeploymentTarget>().getOwnedElements().count(el.getID())) {
        m_el->as<DeploymentTarget>().getOwnedElements().internalRemove(el);
    }

    if (el.as<Deployment>().getLocation()) {
        if (el.as<Deployment>().getLocation()->getID() == m_el->getID()) {
            el.as<Deployment>().setLocation(0);
        }
    }
}

void DeploymentTarget::setManager(UmlManager* manager) {
    m_deployments.m_manager = manager;
}

DeploymentTarget::DeploymentTarget() {
    m_deployments.addProcedures.push_back(new AddDeploymentFunctor(this));
    m_deployments.removeProcedures.push_back(new RemoveDeploymentFunctor(this));
}

DeploymentTarget::DeploymentTarget(const DeploymentTarget& deploymentTarget) {
    m_deployments.addProcedures.clear();
    m_deployments.removeProcedures.clear();
    m_deployments.addProcedures.push_back(new AddDeploymentFunctor(this));
    m_deployments.removeProcedures.push_back(new RemoveDeploymentFunctor(this));
}

DeploymentTarget::~DeploymentTarget() {

}

Sequence<Deployment>& DeploymentTarget::getDeployments() {
    return m_deployments;
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