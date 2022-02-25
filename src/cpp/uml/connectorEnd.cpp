#include "uml/connectorEnd.h"
#include "uml/uml-stable.h"

using namespace UML;

Set<ConnectableElement, ConnectorEnd>& ConnectorEnd::getRoleSingleton() {
    return m_role;
}

Set<Property, ConnectorEnd>& ConnectorEnd::getDefiningEndSingleton() {
    return m_definingEnd;
}

void ConnectorEnd::init() {
    m_role.m_signature = &ConnectorEnd::getRoleSingleton;
    m_role.opposite(&ConnectableElement::getEnds);
    m_definingEnd.m_readOnly = true;
    m_definingEnd.m_signature = &ConnectorEnd::getDefiningEndSingleton;
}

void ConnectorEnd::copy(const ConnectorEnd& rhs) {
    m_role = rhs.m_role;
    m_definingEnd = rhs.m_definingEnd;
}

ConnectorEnd::ConnectorEnd() : Element(ElementType::CONNECTOR_END) {
    init();
}

ConnectorEnd::ConnectorEnd(const ConnectorEnd& rhs) : Element(rhs, ElementType::CONNECTOR_END) {
    init();
    Element::copy(rhs);
    MultiplicityElement::copy(rhs);
    copy(rhs);
    if (!m_copiedElementFlag) {
        delete &rhs;
    }
}

ConnectorEnd::~ConnectorEnd() {
    mountAndRelease();
}

ConnectableElement* ConnectorEnd::getRole() {
    return m_role.get();
}

ConnectableElement& ConnectorEnd::getRoleRef() {
    return m_role.getRef();
}

ID ConnectorEnd::getRoleID() const {
    return m_role.id();
}

bool ConnectorEnd::hasRole() const {
    return m_role.has();
}

void ConnectorEnd::setRole(ConnectableElement* role) {
    m_role.set(role);
}

void ConnectorEnd::setRole(ConnectableElement& role) {
    m_role.set(role);
}

void ConnectorEnd::setRole(ID id) {
    m_role.set(id);
}

Property* ConnectorEnd::getDefiningEnd() {
    return m_definingEnd.get();
}

Property& ConnectorEnd::getDefiningEndRef() {
    return m_definingEnd.getRef();
}

ID ConnectorEnd::getDefiningEndID() const {
    return m_definingEnd.id();
}

bool ConnectorEnd::hasDefiningEnd() const {
    return m_definingEnd.has();
}

bool ConnectorEnd::isSubClassOf(ElementType eType) const {
    bool ret = MultiplicityElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CONNECTOR_END;
    }

    return ret;
}