#include "uml/deployment.h"
#include "uml/deployedArtifact.h"

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

Deployment::Deployment() {
    m_deployedArtifacts.addProcedures.push_back(new AddDeployedArtifactFunctor(this));
    m_deployedArtifacts.removeProcedures.push_back(new RemoveDeployedArtifactFunctor(this));
}

Deployment::Deployment(const Deployment& deployment) {
    m_deployedArtifacts = deployment.m_deployedArtifacts;
    m_deployedArtifacts.addProcedures.clear();
    m_deployedArtifacts.removeProcedures.clear();
    m_deployedArtifacts.addProcedures.push_back(new AddDeployedArtifactFunctor(this));
    m_deployedArtifacts.removeProcedures.push_back(new RemoveDeployedArtifactFunctor(this));
}

Deployment::~Deployment() {

}

Sequence<DeployedArtifact>& Deployment::getDeployedArtifact() {
    return m_deployedArtifacts;
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