#include "uml/types/connectorEnd.h"
#include "uml/types/connectableElement.h"
#include "uml/types/operation.h"
#include "uml/types/manifestation.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<ConnectableElement, ConnectorEnd>& ConnectorEnd::getRoleSingleton() {
    return m_role;
}

TypedSet<Property, ConnectorEnd>& ConnectorEnd::getDefiningEndSingleton() {
    return m_definingEnd;
}

void ConnectorEnd::referenceErased(ID id) {
    MultiplicityElement::referenceErased(id);
    m_role.eraseElement(id);
    m_definingEnd.eraseElement(id);
}

ConnectorEnd::ConnectorEnd() : Element(ElementType::CONNECTOR_END) {
    m_role.opposite(&ConnectableElement::getEnds);
}

ConnectorEnd::~ConnectorEnd() {
    
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