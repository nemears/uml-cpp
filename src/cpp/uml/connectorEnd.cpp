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

TypedSet<ConnectableElement, ConnectorEnd>& ConnectorEnd::getRoleSingleton() {
    return m_role;
}

TypedSet<Property, ConnectorEnd>& ConnectorEnd::getDefiningEndSingleton() {
    return m_definingEnd;
}

void ConnectorEnd::referenceReindexed(ID newID) {
    MultiplicityElement::referenceReindexed(newID);
    m_role.reindex(newID);
    m_definingEnd.reindex(newID);
}

void ConnectorEnd::referenceErased(ID id) {
    MultiplicityElement::referenceErased(id);
    m_role.eraseElement(id);
    m_definingEnd.eraseElement(id);
}

void ConnectorEnd::init() {
    m_role.opposite(&ConnectableElement::getEnds);
    m_definingEnd.m_readOnly = true;
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