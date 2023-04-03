#include "uml/types/connectableElement.h"
#include "uml/types/behavior.h"
#include "uml/types/package.h"
#include "uml/types/stereotype.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

using namespace UML;

void ConnectableElement::referenceReindexed(ID newID) {
    TypedElement::referenceReindexed(newID);
    ParameterableElement::referenceReindexed(newID);
    m_ends.reindex(newID);
}

void ConnectableElement::restoreReference(Element* el) {
    TypedElement::restoreReference(el);
    ParameterableElement::restoreReference(el);
    if (m_ends.contains(el->getID())) {
        el->setReference(this);
    }
    if (el->isSubClassOf(ElementType::CONNECTOR_END)) {
        ConnectorEnd& end = el->as<ConnectorEnd>();
        if (end.getRole().id() == m_id && !m_ends.contains(end.getID())) {
            m_ends.innerAdd(end);
        }
    }
}

void ConnectableElement::referenceErased(ID id) {
    TypedElement::referenceErased(id);
    ParameterableElement::referenceErased(id);
}

ConnectableElement::ConnectableElement() : Element(ElementType::CONNECTABLE_ELEMENT) {
    // m_ends.m_readOnly = true;
    m_ends.opposite(&ConnectorEnd::getRoleSingleton);
}

ConnectableElement::~ConnectableElement() {
    
}

ReadOnlySet<ConnectorEnd, ConnectableElement>& ConnectableElement::getEnds() {
    return m_ends;
}

bool ConnectableElement::isSubClassOf(ElementType eType) const {
    bool ret = TypedElement::isSubClassOf(eType);

    if (!ret) {
        ret = ParameterableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::CONNECTABLE_ELEMENT;
    }

    return ret;
}