#include "uml/uml-stable.h"

using namespace UML;

Singleton<ConnectableElement, ConnectorEnd>& ConnectorEnd::getRoleSingleton() {
    return m_role;
}

Singleton<Property, ConnectorEnd>& ConnectorEnd::getDefiningEndSingleton() {
    return m_definingEnd;
}

void ConnectorEnd::referenceErased(ID id) {
    MultiplicityElement::referenceErased(id);
    eraseFromSet(id, m_role);
    eraseFromSet(id, m_definingEnd);
}

ConnectorEnd::ConnectorEnd() : Element(ElementType::CONNECTOR_END) {
    m_role.opposite(&ConnectableElement::getEnds);
}

ConnectorEnd::~ConnectorEnd() {
    
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

bool ConnectorEnd::is(ElementType eType) const {
    bool ret = MultiplicityElement::is(eType);

    if (!ret) {
        ret = eType == ElementType::CONNECTOR_END;
    }

    return ret;
}
