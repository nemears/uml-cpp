#include "uml/connector.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
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
    
}

void Connector::referenceErased(ID id) {
    Feature::referenceErased(id);
    m_type.eraseElement(id);
    m_contracts.eraseElement(id);
}

void Connector::init() {
    m_ends.subsets(*m_ownedElements);
    m_structuredClassifier.subsets(m_featuringClassifier);
    m_structuredClassifier.subsets(*m_namespace);
    m_structuredClassifier.opposite(&StructuredClassifier::getOwnedConnectors);
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