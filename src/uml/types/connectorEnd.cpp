#include "uml/uml-stable.h"

using namespace UML;

Singleton<ConnectableElement, ConnectorEnd>& ConnectorEnd::getRoleSingleton() {
    return m_role;
}

Singleton<Property, ConnectorEnd>& ConnectorEnd::getDefiningEndSingleton() {
    return m_definingEnd;
}

ConnectorEnd::ConnectorEnd(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    MultiplicityElement(elementType, manager)
{
    m_role.opposite(&ConnectableElement::getEnds);
}

ConnectableElementPtr ConnectorEnd::getRole() const {
    return m_role.get();
}

void ConnectorEnd::setRole(ConnectableElementPtr role) {
    m_role.set(role);
}

void ConnectorEnd::setRole(ConnectableElement& role) {
    m_role.set(role);
}

void ConnectorEnd::setRole(ID id) {
    m_role.set(id);
}

PropertyPtr ConnectorEnd::getDefiningEnd() const {
    return m_definingEnd.get();
}
