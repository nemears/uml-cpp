#include "uml/deploymentTarget.h"
#include "uml/deployment.h"

using namespace UML;

void DeploymentTarget::AddDeploymentFunctor::operator()(Deployment& el) const {
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
    if (!m_el->getClientDependencies().count(el.getID())) {
        m_el->getClientDependencies().add(el);
    }

    el.setLocation(m_el);
    updateCopiedSequenceAddedTo(el, &DeploymentTarget::getDeployments);
}
void DeploymentTarget::AddDeploymentFunctor::operator()(ID id) const {
    if (!m_el->getOwnedElements().count(id)) {
        m_el->getOwnedElements().addByID(id);
    }
    if (!m_el->getClientDependencies().count(id)) {
        m_el->getClientDependencies().addByID(id);
    }
}

void DeploymentTarget::RemoveDeploymentFunctor::operator()(Deployment& el) const {
    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }

    if (el.getLocation()) {
        if (el.getLocation()->getID() == m_el->getID()) {
            el.setLocation(0);
        }
    }
    updateCopiedSequenceRemovedFrom(el, &DeploymentTarget::getDeployments);
}

void DeploymentTarget::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    m_deployments.elementReleased(id, &DeploymentTarget::getDeployments);
}

void DeploymentTarget::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    m_deployments.reindex(oldID, newID, &DeploymentTarget::getDeployments);
}

void DeploymentTarget::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    m_deployments.elementErased(id);
}

DeploymentTarget::DeploymentTarget() : Element(ElementType::DEPLOYMENT_TARGET) {
    m_deployments.addProcedures.push_back(new AddDeploymentFunctor(this));
    m_deployments.removeProcedures.push_back(new RemoveDeploymentFunctor(this));
}

DeploymentTarget::DeploymentTarget(const DeploymentTarget& deploymentTarget) : Element(deploymentTarget, ElementType::DEPLOYMENT_TARGET) {
    m_deployments = deploymentTarget.m_deployments;
    m_deployments.m_el = this;
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

bool DeploymentTarget::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DEPLOYMENT_TARGET;
    }

    return ret;
}