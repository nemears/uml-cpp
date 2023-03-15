#include "uml/types/connector.h"
#include "uml/types/operation.h"
#include "uml/types/manifestation.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Connector::SetTypePolicy::apply(Association& el, Connector& me) {
    for (auto& end : me.getEnds()) {
        if (end.getRole() && end.getRole()->getType().id() != ID::nullID()) {
            SetLock endLck = me.lockEl(end);
            for (auto& assocEnd : el.getMemberEnds()) {
                if (assocEnd.getType().id() == end.getRole()->getType().id()) {
                    SetLock assocEndLock = me.lockEl(assocEnd);
                    end.m_definingEnd.innerAdd(assocEnd);
                    break;
                }
            }
        }
    }
}

void Connector::RemoveTypePolicy::apply(Association& el, Connector& me) {
    // TODO
}

void Connector::AddEndPolicy::apply(ConnectorEnd& el, Connector& me) {
    if (me.getType()) {
        if (el.getRole() && el.getRole()->getType()) {
            for (auto& assocEnd : me.getType()->getMemberEnds()) {
                if (assocEnd.getType().id() == el.getRole()->getType().id()) {
                    SetLock assocEndLck = me.lockEl(assocEnd);
                    el.m_definingEnd.innerAdd(assocEnd);
                    break;
                }
            }
        }
    }
}

void Connector::RemoveEndPolicy::apply(ConnectorEnd& el, Connector& me) {
    // TODO
}

TypedSet<Association, Connector>& Connector::getTypeSingleton() {
    return m_type;
}

TypedSet<StructuredClassifier, Connector>& Connector::getStructuredClassifierSingleton() {
    return m_structuredClassifier;
}

void Connector::referenceReindexed(ID newID) {
    Feature::referenceReindexed(newID);
    m_type.reindex(newID);
    m_contracts.reindex(newID);
}

void Connector::restoreReference(Element* el) {
    Feature::restoreReference(el);
    if (!m_structuredClassifier.get() &&
        m_featuringClassifier.get() && 
        m_featuringClassifier.get().id() == el->getID()) 
    {
        if (el->isSubClassOf(ElementType::STRUCTURED_CLASSIFIER)) {
            // TODO lock
            m_structuredClassifier.innerAdd(el->as<StructuredClassifier>());
        }
    }
    
    if (
            m_ends.contains(el->getID()) && 
            el->as<ConnectorEnd>().getRole() && 
            el->as<ConnectorEnd>().getRole()->getType() && 
            m_type.get()
        )
    {
        for (auto& assocEnd : m_type.get()->getMemberEnds()) {
            if (assocEnd.getType().id() == el->as<ConnectorEnd>().getRole()->getType().id() && !el->as<ConnectorEnd>().getDefiningEnd()) {
                el->as<ConnectorEnd>().m_definingEnd.innerAdd(assocEnd);
            }
        }
    }
}

void Connector::referenceErased(ID id) {
    Feature::referenceErased(id);
    m_type.eraseElement(id);
    m_contracts.eraseElement(id);
}

Connector::Connector() : Element(ElementType::CONNECTOR) {
    m_ends.subsets(*m_ownedElements);
    m_structuredClassifier.subsets(m_featuringClassifier);
    m_structuredClassifier.subsets(*m_namespace);
    m_structuredClassifier.opposite(&StructuredClassifier::getOwnedConnectors);
}

Connector::~Connector() {
    
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