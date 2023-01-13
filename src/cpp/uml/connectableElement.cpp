#include "uml/connectableElement.h"
#include "uml/behavior.h"
#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"

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
}

void ConnectableElement::referenceErased(ID id) {
    TypedElement::referenceErased(id);
    ParameterableElement::referenceErased(id);
}

ConnectableElement::ConnectableElement() : Element(ElementType::CONNECTABLE_ELEMENT) {
    m_ends.m_readOnly = true;
    m_ends.opposite(&ConnectorEnd::getRoleSingleton);
}

ConnectableElement::~ConnectableElement() {
    
}

Set<ConnectorEnd, ConnectableElement>& ConnectableElement::getEnds() {
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