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