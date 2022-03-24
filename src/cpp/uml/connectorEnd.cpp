#include "uml/connectorEnd.h"
#include "uml/connectableElement.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

Set<ConnectableElement, ConnectorEnd>& ConnectorEnd::getRoleSingleton() {
    return m_role;
}

Set<Property, ConnectorEnd>& ConnectorEnd::getDefiningEndSingleton() {
    return m_definingEnd;
}

void ConnectorEnd::init() {
    m_role.opposite(&ConnectableElement::getEnds);
    m_role.m_signature = &ConnectorEnd::getRoleSingleton;
    m_definingEnd.m_readOnly = true;
    m_definingEnd.m_signature = &ConnectorEnd::getDefiningEndSingleton;
}

ConnectorEnd::ConnectorEnd() : Element(ElementType::CONNECTOR_END) {
    init();
}

ConnectorEnd::~ConnectorEnd() {
    mountAndRelease();
}

ConnectableElementPtr ConnectorEnd::getRole() const {
    return m_role.get();
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

PropertyPtr ConnectorEnd::getDefiningEnd() const {
    return m_definingEnd.get();
}

bool ConnectorEnd::isSubClassOf(ElementType eType) const {
    bool ret = MultiplicityElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CONNECTOR_END;
    }

    return ret;
}