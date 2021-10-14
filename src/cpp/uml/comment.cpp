#include "uml/comment.h"
#include "uml/sequence.h"

using namespace std;
using namespace UML;

void Comment::RemoveOwningElementProcedure::operator()(Element* el) const {
    if (el->getOwnedComments().count(m_me->getID())) {
        el->getOwnedComments().remove(*m_me);
    }
}

void Comment::AddOwningElementProcedure::operator()(Element* el) const {
    if (!el->getOwnedComments().count(m_me->getID())) {
        el->getOwnedComments().add(*m_me);
    }
    if (m_me->getOwnerID() != el->getID()) {
        m_me->setOwner(el);
    }
}

void Comment::AddOwningElementProcedure::operator()(ID id) const {
    if (m_me->getOwnerID() != id) {
        m_me->setOwnerByID(id);
    }
}

void Comment::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    if (m_owningElement.id() == oldID) {
        m_owningElement.reindex(oldID, newID);
    }
}

void Comment::referencingReleased(ID id) {
    Element::referencingReleased(id);
    if (m_owningElement.id() == id) {
        m_owningElement.release();
    }
}

void Comment::restoreReferences() {
    Element::restoreReferences();
    m_owningElement.restoreReference();
}

void Comment::referenceErased(ID id) {
    Element::referenceErased(id);
    m_owningElement.elementErased(id);
}

Comment::Comment() {
    m_owningElement.m_signature = &Comment::m_owningElement;
    m_owningElement.m_removeProcedures.push_back(new RemoveOwningElementProcedure(this));
    m_owningElement.m_addProcedures.push_back(new AddOwningElementProcedure(this));
}

Comment::Comment(const Comment& lhs) : Element(lhs) {
    m_owningElement = lhs.m_owningElement;
    m_owningElement.m_me = this;
    m_owningElement.m_addProcedures.clear();
    m_owningElement.m_removeProcedures.clear();
    m_owningElement.m_removeProcedures.push_back(new RemoveOwningElementProcedure(this));
    m_owningElement.m_addProcedures.push_back(new AddOwningElementProcedure(this));
}

string Comment::getBody() {
    return m_body;
}

void Comment::setBody(string body) {
    m_body = body;
}

Element* Comment::getOwningElement() {
    return m_owningElement.get();
}

Element& Comment::getOwningElementRef() {
    return m_owningElement.getRef();
}

ID Comment::getOwningElementID() const {
    return m_owningElement.id();
}

bool Comment::hasOwningElement() const {
    return m_owningElement.has();
}

void Comment::setOwningElement(Element* el) {
    m_owningElement.set(el);
}

void Comment::setOwningElement(Element& el) {
    m_owningElement.set(el);
}

ElementType Comment::getElementType() const {
    return ElementType::COMMENT;
}

bool Comment::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::COMMENT;
    }

    return ret;
}