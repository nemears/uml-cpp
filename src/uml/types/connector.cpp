#include "uml/types/connector.h"
#include "uml/set/singleton.h"
#include "uml/uml-stable.h"

using namespace UML;

void Connector::TypePolicy::elementAdded(Association& el, Connector& me) {
    for (auto& end : me.getEnds()) {
        if (end.getRole() && end.getRole()->getType().id() != ID::nullID()) {
            // __attribute__((unused)) SetLock endLck = me.lockEl(end);
            for (auto& assocEnd : el.getMemberEnds()) {
                if (assocEnd.getType().id() == end.getRole()->getType().id()) {
                    // __attribute__((unused)) SetLock assocEndLock = me.lockEl(assocEnd);
                    end.m_definingEnd.innerAdd(&assocEnd);
                    break;
                }
            }
        }
    }
}

void Connector::TypePolicy::elementRemoved(Association& el, Connector& me) {
    // TODO
}

void Connector::EndPolicy::elementAdded(ConnectorEnd& el, Connector& me) {
    if (me.getType()) {
        if (el.getRole() && el.getRole()->getType()) {
            for (auto& assocEnd : me.getType()->getMemberEnds()) {
                if (assocEnd.getType().id() == el.getRole()->getType().id()) {
                    // __attribute__((unused)) SetLock assocEndLck = me.lockEl(assocEnd);
                    el.m_definingEnd.innerAdd(&assocEnd);
                    break;
                }
            }
        }
    }
}

void Connector::EndPolicy::elementRemoved(ConnectorEnd& el, Connector& me) {
    // TODO
}

Singleton<Association, Connector, Connector::TypePolicy>& Connector::getTypeSingleton() {
    return m_type;
}

void Connector::referenceErased(ID id) {
    Feature::referenceErased(id);
    eraseFromSet(id, m_contracts);
    eraseFromSet(id, m_type);
}

void Connector::restoreReferences() {
    if (m_namespace.get() && !m_featuringClassifier.get()) {
        if (m_namespace.get()->is(ElementType::STRUCTURED_CLASSIFIER)) {
            StructuredClassifier& clazz = m_namespace.get()->as<StructuredClassifier>();
            if (clazz.getOwnedConnectors().contains(m_id) && !m_featuringClassifier.get()) {
                m_featuringClassifier.m_structure->m_rootRedefinedSet->m_set.nonOppositeAdd(&clazz);
            }
        }
    }
    if (m_type.get()) {
        for (auto& connectorEnd : m_ends) {
            if (connectorEnd.getRole() && !connectorEnd.getDefiningEnd()) {
                for (auto& memberEnd : m_type.get()->getMemberEnds()) {
                    if (memberEnd.getType().id() == connectorEnd.getRole()->getType().id()) {
                        connectorEnd.m_definingEnd.innerAdd(&memberEnd);
                    }
                }
            }
        }
    }
}

Connector::Connector() : Element(ElementType::CONNECTOR) {
    m_ends.subsets(*m_ownedElements);
}

Connector::~Connector() {
    
}

AssociationPtr Connector::getType() const {
    return m_type.get();
}

void Connector::setType(AssociationPtr type) {
    m_type.set(type);
}

void Connector::setType(Association& type) {
    m_type.set(type);
}

void Connector::setType(ID id) {
    m_type.set(id);
}

IndexableSet<Behavior, Connector>& Connector::getContracts() {
    return m_contracts;
}

IndexableOrderedSet<ConnectorEnd, Connector, Connector::EndPolicy>& Connector::getEnds() {
    return m_ends;
}

bool Connector::is(ElementType eType) const {
    bool ret = Feature::is(eType);

    if (!ret) {
        ret = eType == ElementType::CONNECTOR;
    }

    return ret;
}
