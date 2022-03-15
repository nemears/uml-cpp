#include "uml/connectableElement.h"
#include "uml/setReferenceFunctor.h"
#include "uml/behavior.h"
#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

void ConnectableElement::referencingReleased(ID id) {
    TypedElement::referencingReleased(id);
    ParameterableElement::referencingReleased(id);
    m_ends.release(id);
}

void ConnectableElement::referenceReindexed(ID oldID, ID newID) {
    TypedElement::referenceReindexed(oldID, newID);
    ParameterableElement::referenceReindexed(oldID, newID);
    m_ends.reindex(oldID, newID);
}

void ConnectableElement::reindexName(ID id, std::string newName) {
    TypedElement::reindexName(id, newName);
    ParameterableElement::reindexName(id, newName);
    m_ends.reindexName(id, newName);
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

void ConnectableElement::init() {
    m_ends.m_readOnly = true;
    m_ends.m_signature = &ConnectableElement::getEnds;
    m_ends.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_ends.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
}

ConnectableElement::ConnectableElement() : Element(ElementType::CONNECTABLE_ELEMENT) {
    init();
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