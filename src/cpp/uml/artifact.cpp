#include "uml/artifact.h"
#include "uml/property.h"
#include "uml/operation.h"
#include "uml/manifestation.h"

using namespace UML;

void Artifact::setManager(UmlManager* manager) {
    Classifier::setManager(manager);
    DeployedArtifact::setManager(manager);
    m_ownedAttributes.m_manager = manager;
    m_ownedOperations.m_manager = manager;
}

void Artifact::AddOwnedAttributeFunctor::operator()(Element& el) const {
    if (!dynamic_cast<Artifact*>(m_el)->getAttributes().count(el.getID())) {
        dynamic_cast<Artifact*>(m_el)->getAttributes().add(dynamic_cast<Property&>(el));
    }

    if (!dynamic_cast<Artifact*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<Artifact*>(m_el)->getOwnedMembers().add(dynamic_cast<Property&>(el));
    }

    if (dynamic_cast<Property&>(el).getArtifact() != m_el) {
        dynamic_cast<Property&>(el).setArtifact(dynamic_cast<Artifact*>(m_el));
    }
}

void Artifact::RemoveOwnedAttributeFunctor::operator()(Element& el) const {
    if (dynamic_cast<Artifact*>(m_el)->getAttributes().count(el.getID())) {
        dynamic_cast<Artifact*>(m_el)->getAttributes().remove(dynamic_cast<Property&>(el));
    }

    if (dynamic_cast<Artifact*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<Artifact*>(m_el)->getOwnedMembers().remove(dynamic_cast<Property&>(el));
    }

    if (dynamic_cast<Property&>(el).getArtifact() == m_el) {
        dynamic_cast<Property&>(el).setArtifact(0);
    }
}

void Artifact::AddOwnedOperationFunctor::operator()(Element& el) const {
    if (!dynamic_cast<Artifact*>(m_el)->getFeatures().count(el.getID())) {
        dynamic_cast<Artifact*>(m_el)->getFeatures().add(dynamic_cast<Operation&>(el));
    }

    if (!dynamic_cast<Artifact*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<Artifact*>(m_el)->getOwnedMembers().add(dynamic_cast<Operation&>(el));
    }

    if (dynamic_cast<Operation&>(el).getArtifact() != m_el) {
        dynamic_cast<Operation&>(el).setArtifact(dynamic_cast<Artifact*>(m_el));
    }

    if (!dynamic_cast<Operation&>(el).getRedefinitionContext().count(m_el->getID())) {
        dynamic_cast<Operation&>(el).getRedefinitionContext().add(*dynamic_cast<Artifact*>(m_el));
    }
}

void Artifact::RemoveOwnedOperationFunctor::operator()(Element& el) const {
    if (dynamic_cast<Artifact*>(m_el)->getFeatures().count(el.getID())) {
        dynamic_cast<Artifact*>(m_el)->getFeatures().remove(dynamic_cast<Operation&>(el));
    }

    if (dynamic_cast<Artifact*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<Artifact*>(m_el)->getOwnedMembers().remove(dynamic_cast<Operation&>(el));
    }

    if (dynamic_cast<Operation&>(el).getArtifact() == m_el) {
        dynamic_cast<Operation&>(el).setArtifact(0);
    }

    if (dynamic_cast<Operation&>(el).getRedefinitionContext().count(m_el->getID())) {
        dynamic_cast<Operation&>(el).getRedefinitionContext().remove(*dynamic_cast<Artifact*>(m_el));
    }
}

void Artifact::CheckNestedArtifactFunctor::operator()(Element& el) const {
    if (el.getID() == m_el->getID()) {
        throw NestedArtifactException();
    }
}

void Artifact::AddNestedArtifactFunctor::operator()(Element& el) const {
    if (!m_el->as<Artifact>().getOwnedMembers().count(el.getID())) {
        m_el->as<Artifact>().getOwnedMembers().add(el.as<Artifact>());
    }
}

void Artifact::RemoveNestedArtifactFunctor::operator()(Element& el) const {
    if (m_el->as<Artifact>().getOwnedMembers().count(el.getID())) {
        m_el->as<Artifact>().getOwnedMembers().remove(el.as<Artifact>());
    }
}

void Artifact::AddManifestationFunctor::operator()(Element& el) const {
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }

    el.as<Manifestation>().setArtifact(&m_el->as<Artifact>()); 
}

void Artifact::RemoveManifestationFunctor::operator()(Element& el) const {
    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }

    el.as<Manifestation>().setArtifact(0); 
}

Artifact::Artifact() {
    m_ownedAttributes.addProcedures.push_back(new AddOwnedAttributeFunctor(this));
    m_ownedAttributes.removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
    m_ownedOperations.addProcedures.push_back(new AddOwnedOperationFunctor(this));
    m_ownedOperations.removeProcedures.push_back(new RemoveOwnedOperationFunctor(this));
    m_nestedArtifacts.addProcedures.push_back(new AddNestedArtifactFunctor(this));
    m_nestedArtifacts.removeProcedures.push_back(new RemoveNestedArtifactFunctor(this));
    m_nestedArtifacts.addChecks.push_back(new CheckNestedArtifactFunctor(this));
    m_manifestations.addProcedures.push_back(new AddManifestationFunctor(this));
    m_manifestations.removeProcedures.push_back(new RemoveManifestationFunctor(this));
}

Artifact::Artifact(const Artifact& artifact) {
    m_ownedAttributes = artifact.m_ownedAttributes;
    m_ownedAttributes.addProcedures.clear();
    m_ownedAttributes.addProcedures.push_back(new AddOwnedAttributeFunctor(this));
    m_ownedAttributes.removeProcedures.clear();
    m_ownedAttributes.removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
    m_ownedOperations = artifact.m_ownedOperations;
    m_ownedOperations.addProcedures.clear();
    m_ownedOperations.addProcedures.push_back(new AddOwnedOperationFunctor(this));
    m_ownedOperations.removeProcedures.clear();
    m_ownedOperations.removeProcedures.push_back(new RemoveOwnedOperationFunctor(this));
    m_nestedArtifacts = artifact.m_nestedArtifacts;
    m_nestedArtifacts.addProcedures.clear();
    m_nestedArtifacts.removeProcedures.clear();
    m_nestedArtifacts.addProcedures.push_back(new AddNestedArtifactFunctor(this));
    m_nestedArtifacts.removeProcedures.push_back(new RemoveNestedArtifactFunctor(this));
    m_nestedArtifacts.addChecks.clear();
    m_nestedArtifacts.addChecks.push_back(new CheckNestedArtifactFunctor(this));
    m_manifestations = artifact.m_manifestations;
    m_manifestations.addProcedures.clear();
    m_manifestations.removeProcedures.clear();
    m_manifestations.addProcedures.push_back(new AddManifestationFunctor(this));
    m_manifestations.removeProcedures.push_back(new RemoveManifestationFunctor(this));
}

Sequence<Property>& Artifact::getOwnedAttributes() {
    return m_ownedAttributes;
}

Sequence<Operation>& Artifact::getOwnedOperations() {
    return m_ownedOperations;
}

Sequence<Artifact>& Artifact::getNestedArtifacts() {
    return m_nestedArtifacts;
}

Sequence<Manifestation>& Artifact::getManifestations() {
    return m_manifestations;
}

Artifact::~Artifact() {

}

ElementType Artifact::getElementType() const {
    return ElementType::ARTIFACT;
}

bool Artifact::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = DeployedArtifact::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::ARTIFACT;
    }

    return ret;
}