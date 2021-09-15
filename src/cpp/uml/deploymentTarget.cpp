#include "uml/deploymentTarget.h"
#include "uml/deployment.h"

using namespace UML;

void DeploymentTarget::AddDeploymentFunctor::operator()(Deployment& el) const {
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }

    el.setLocation(m_el);
    updateCopiedSequenceAddedTo(el, &DeploymentTarget::getDeployments);
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

void DeploymentTarget::setManager(UmlManager* manager) {
    NamedElement::setManager(manager);
    m_deployments.m_manager = manager;
}

void DeploymentTarget::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    m_deployments.elementReleased(id, &DeploymentTarget::getDeployments);
}

void DeploymentTarget::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    if (m_deployments.count(oldID)) {
        m_deployments.reindex(oldID, newID, &DeploymentTarget::getDeployments);
    }
}

DeploymentTarget::DeploymentTarget() {
    m_deployments.addProcedures.push_back(new AddDeploymentFunctor(this));
    m_deployments.removeProcedures.push_back(new RemoveDeploymentFunctor(this));
}

DeploymentTarget::DeploymentTarget(const DeploymentTarget& deploymentTarget) {
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