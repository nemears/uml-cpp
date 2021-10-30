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

    if (m_me->getOwnerID() != el->getID()) {
        m_me->setOwner(el);
    }
}

void Deployment::AddLocationProcedure::operator()(ID id) const {
    if (!m_me->m_client.count(id)) {
        m_me->m_client.addByID(id);
    }
    if (m_me->getOwnerID() != id) {
        m_me->setOwnerByID(id);
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

void Deployment::referencingReleased(ID id) {
    Dependency::referencingReleased(id);
    m_location.release(id);
    m_deployedArtifacts.elementReleased(id, &Deployment::getDeployedArtifact);
}

void Deployment::referenceReindexed(ID oldID, ID newID) {
    Dependency::referenceReindexed(oldID, newID);
    m_location.reindex(oldID, newID);
    m_deployedArtifacts.reindex(oldID, newID, &Deployment::getDeployedArtifact);
}

void Deployment::restoreReferences() {
    Dependency::restoreReferences();
    m_location.restoreReference();
    m_deployedArtifacts.restoreReferences();
}

void Deployment::referenceErased(ID id) {
    Dependency::referenceErased(id);
    m_location.elementErased(id);
    m_deployedArtifacts.elementErased(id);
}

Deployment::Deployment() : Element(ElementType::DEPLOYMENT) {
    m_deployedArtifacts.addProcedures.push_back(new AddDeployedArtifactFunctor(this));
    m_deployedArtifacts.removeProcedures.push_back(new RemoveDeployedArtifactFunctor(this));
    m_location.m_signature = &Deployment::m_location;
    m_location.m_removeProcedures.push_back(new RemoveLocationProcedure(this));
    m_location.m_addProcedures.push_back(new AddLocationProcedure(this));
}

Deployment::Deployment(const Deployment& deployment) : Element(ElementType::DEPLOYMENT) {
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

ID Deployment::getLocationID() const {
    return m_location.id();
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

bool Deployment::isSubClassOf(ElementType eType) const {
    bool ret = Dependency::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DEPLOYMENT;
    }

    return ret;
}