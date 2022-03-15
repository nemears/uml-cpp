#include "uml/connector.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/setReferenceFunctor.h"
#include "uml/umlPtr.h"

using namespace UML;

void Connector::SetTypeFunctor::operator()(Element& el) const {
    for (auto& end : m_el.as<Connector>().getEnds()) {
        if (end.getRole() && end.getRole()->getType().id() != ID::nullID()) {
            for (auto& assocEnd : el.as<Association>().getMemberEnds()) {
                if (assocEnd.getType().id() == end.getRole()->getType().id()) {
                    end.m_definingEnd.nonOppositeAdd(assocEnd);
                    break;
                }
            }
        }
    }
}

void Connector::RemoveTypeFunctor::operator()(Element& el) const {

}

void Connector::AddEndFunctor::operator()(Element& el) const {
    if (m_el.as<Connector>().getType()) {
        if (el.as<ConnectorEnd>().getRole() && el.as<ConnectorEnd>().getRole()->getType()) {
            for (auto& assocEnd : m_el.as<Connector>().getType()->getMemberEnds()) {
                if (assocEnd.getType().id() == el.as<ConnectorEnd>().getRole()->getType().id()) {
                    el.as<ConnectorEnd>().m_definingEnd.nonOppositeAdd(assocEnd);
                    break;
                }
            }
        }
    }
}

void Connector::RemoveEndFunctor::operator()(Element& el) const {

}

Set<Association, Connector>& Connector::getTypeSingleton() {
    return m_type;
}

Set<ConnectorEnd, Connector>& Connector::getEndsSet() {
    return m_ends;
}

void Connector::referencingReleased(ID id) {
    Feature::referencingReleased(id);
    m_type.release(id);
    m_contracts.release(id);
}

void Connector::referenceReindexed(ID oldID, ID newID) {
    Feature::referenceReindexed(oldID, newID);
    m_type.reindex(oldID, newID);
    m_contracts.reindex(oldID, newID);
}

void Connector::reindexName(ID id, std::string newName) {
    Feature::reindexName(id, newName);
    m_type.reindexName(id, newName);
    m_contracts.reindexName(id, newName);
}

void Connector::restoreReference(Element* el) {
    Feature::restoreReference(el);
    if (m_type.get().id() == el->getID()) {
        el->setReference(this);
    } else if (m_contracts.contains(el->getID())) {
        el->setReference(this);
    }
}

void Connector::referenceErased(ID id) {
    Feature::referenceErased(id);
    m_type.eraseElement(id);
    m_contracts.eraseElement(id);
}

void Connector::init() {
    m_type.m_signature = &Connector::getTypeSingleton;
    m_type.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_type.m_addFunctors.insert(new SetTypeFunctor(this));
    m_type.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_type.m_removeFunctors.insert(new RemoveTypeFunctor(this));
    m_contracts.m_signature = &Connector::getContracts;
    m_contracts.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_contracts.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_ends.subsets(*m_ownedElements);
    m_ends.m_signature = &Connector::getEndsSet;
    m_ends.m_addFunctors.insert(new AddEndFunctor(this));
    m_ends.m_removeFunctors.insert(new RemoveEndFunctor(this));
}

Connector::Connector() : Element(ElementType::CONNECTOR) {
    init();
}

Connector::~Connector() {
    mountAndRelease();
}

AssociationPtr Connector::getType() const {
    return m_type.get();
}

void Connector::setType(Association* type) {
    m_type.set(type);
}

void Connector::setType(Association& type) {
    m_type.set(type);
}

void Connector::setType(ID id) {
    m_type.set(id);
}

Set<Behavior, Connector>& Connector::getContracts() {
    return m_contracts;
}

OrderedSet<ConnectorEnd, Connector>& Connector::getEnds() {
    return m_ends;
}

bool Connector::isSubClassOf(ElementType eType) const {
    bool ret = Feature::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CONNECTOR;
    }

    return ret;
}