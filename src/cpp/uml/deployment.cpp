#include "uml/deployment.h"
#include "uml/deployedArtifact.h"
#include "uml/deploymentTarget.h"

using namespace UML;

void Deployment::AddDeployedArtifactFunctor::operator()(Element& el) const {
    if (!m_el->as<Deployment>().getSupplier().count(el.getID())) {
        m_el->as<Deployment>().getSupplier().add(el.as<DeployedArtifact>());
    }
}

void Deployment::RemoveDeployedArtifactFunctor::operator()(Element& el) const {
    if (m_el->as<Deployment>().getSupplier().count(el.getID())) {
        m_el->as<Deployment>().getSupplier().remove(el.as<DeployedArtifact>());
    }
}

void Deployment::setManager(UmlManager* manager) {
    m_deployedArtifacts.m_manager = manager;
}

Deployment::Deployment() {
    m_deployedArtifacts.addProcedures.push_back(new AddDeployedArtifactFunctor(this));
    m_deployedArtifacts.removeProcedures.push_back(new RemoveDeployedArtifactFunctor(this));
    m_locationPtr = 0;
}

Deployment::Deployment(const Deployment& deployment) {
    m_deployedArtifacts = deployment.m_deployedArtifacts;
    m_deployedArtifacts.addProcedures.clear();
    m_deployedArtifacts.removeProcedures.clear();
    m_deployedArtifacts.addProcedures.push_back(new AddDeployedArtifactFunctor(this));
    m_deployedArtifacts.removeProcedures.push_back(new RemoveDeployedArtifactFunctor(this));
    m_locationID = deployment.m_locationID;
    if (!m_manager) {
        m_locationPtr = deployment.m_locationPtr;
    }
}

Deployment::~Deployment() {

}

Sequence<DeployedArtifact>& Deployment::getDeployedArtifact() {
    return m_deployedArtifacts;
}

DeploymentTarget* Deployment::getLocation() {
    if (!m_locationID.isNull()) {
        if (!m_locationPtr) {
            m_locationPtr = &m_manager->get<DeploymentTarget>(m_locationID);
        }
        return m_locationPtr;
    }
    return 0;
}

void Deployment::setLocation(DeploymentTarget* location) {
    if (!isSameOrNull(m_locationID, location)) {
        if (!m_locationPtr) {
            m_locationPtr = &m_manager->get<DeploymentTarget>(m_locationID);
        }

        if (m_locationPtr->getDeployments().count(m_id)) {
            m_locationPtr->getDeployments().remove(*this);
        }

        if (m_client.count(m_locationID)) {
            m_client.remove(*m_locationPtr);
        }

        m_locationID = ID::nullID();
        m_locationPtr = 0;
    }

    if (location) {
        m_locationID = location->getID();
    }

    if (!m_manager) {
        m_locationPtr = location;
    }

    if (location) {
        if (!m_client.count(location->getID())) {
            m_client.add(*location);
        }

        if (!location->getDeployments().count(m_id)) {
            location->getDeployments().add(*this);
        }
    }
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