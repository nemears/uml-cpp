#include "uml/deployment.h"
#include "uml/deployedArtifact.h"
#include "uml/deploymentTarget.h"

using namespace UML;

void Deployment::RemoveLocationProcedure::operator()(DeploymentTarget* el) const {
    if (el->getDeployments().count(m_me->getID())) {
        el->getDeployments().remove(*m_me);
    }

    if (m_me->m_client.count(el->getID())) {
        m_me->m_client.remove(*el);
    }
}

void Deployment::AddLocationProcedure::operator()(DeploymentTarget* el) const {
    if (!m_me->m_client.count(el->getID())) {
        m_me->m_client.add(*el);
    }

    if (!el->getDeployments().count(m_me->getID())) {
        el->getDeployments().add(*m_me);
    }
}

void Deployment::AddDeployedArtifactFunctor::operator()(DeployedArtifact& el) const {
    if (!m_el->getSupplier().count(el.getID())) {
        m_el->getSupplier().add(el);
    }
}

void Deployment::RemoveDeployedArtifactFunctor::operator()(DeployedArtifact& el) const {
    if (m_el->getSupplier().count(el.getID())) {
        m_el->getSupplier().remove(el);
    }
}

void Deployment::setManager(UmlManager* manager) {
    Dependency::setManager(manager);
    m_deployedArtifacts.m_manager = manager;
}

void Deployment::referencingReleased(ID id) {
    Dependency::referencingReleased(id);
    if (m_location.id() == id) {
        m_location.release();
    }
    m_deployedArtifacts.elementReleased(id, &Deployment::getDeployedArtifact);
}

void Deployment::referenceReindexed(ID oldID, ID newID) {
    Dependency::referenceReindexed(oldID, newID);
    if (m_location.id() == oldID) {
        m_location.reindex(oldID, newID);
    }
    if (m_deployedArtifacts.count(oldID)) {
        m_deployedArtifacts.reindex(oldID, newID, &Deployment::getDeployedArtifact);
    }
}

Deployment::Deployment() {
    m_deployedArtifacts.addProcedures.push_back(new AddDeployedArtifactFunctor(this));
    m_deployedArtifacts.removeProcedures.push_back(new RemoveDeployedArtifactFunctor(this));
    m_location.m_signature = &Deployment::m_location;
    m_location.m_removeProcedures.push_back(new RemoveLocationProcedure(this));
    m_location.m_addProcedures.push_back(new AddLocationProcedure(this));
}

Deployment::Deployment(const Deployment& deployment) {
    m_deployedArtifacts = deployment.m_deployedArtifacts;
    m_deployedArtifacts.addProcedures.clear();
    m_deployedArtifacts.removeProcedures.clear();
    m_deployedArtifacts.addProcedures.push_back(new AddDeployedArtifactFunctor(this));
    m_deployedArtifacts.removeProcedures.push_back(new RemoveDeployedArtifactFunctor(this));
    m_location = deployment.m_location;
    m_location.m_me = this;
    m_location.m_removeProcedures.clear();
    m_location.m_addProcedures.clear();
    m_location.m_removeProcedures.push_back(new RemoveLocationProcedure(this));
    m_location.m_addProcedures.push_back(new AddLocationProcedure(this));
}

Deployment::~Deployment() {

}

Sequence<DeployedArtifact>& Deployment::getDeployedArtifact() {
    return m_deployedArtifacts;
}

DeploymentTarget* Deployment::getLocation() {
    return m_location.get();
}

DeploymentTarget& Deployment::getLocationRef() {
    return m_location.getRef();
}

bool Deployment::hasLocation() const {
    return m_location.has();
}

void Deployment::setLocation(DeploymentTarget* location) {
    m_location.set(location);
}

void Deployment::setLocation(DeploymentTarget& location) {
    m_location.set(location);
}

ElementType Deployment::getElementType() const {
    return ElementType::DEPLOYMENT;
}

bool Deployment::isSubClassOf(ElementType eType) const {
    bool ret = Dependency::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DEPLOYMENT;
    }

    return ret;
}