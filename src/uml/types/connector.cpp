#include "uml/uml-stable.h"

using namespace UML;

void Connector::TypePolicy::elementAdded(Association& el, Connector& me) {
    for (auto& end : me.getEnds()) {
        if (end.getRole() && end.getRole()->getType().id() != ID::nullID()) {
            // __attribute__((unused)) SetLock endLck = me.lockEl(end);
            for (auto& assocEnd : el.getMemberEnds()) {
                if (assocEnd.getType().id() == end.getRole()->getType().id()) {
                    me.addToReadonlySet(end.m_definingEnd, assocEnd);
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
                    me.addToReadonlySet(el.m_definingEnd, assocEnd);
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

Connector::Connector(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    RedefinableElement(elementType, manager),
    Feature(elementType, manager)
{
    m_ends.subsets(m_ownedElements);
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
