#include "uml/comment.h"
#include "uml/sequence.h"
#include "uml/universalFunctions.h"

using namespace std;
using namespace UML;

void Comment::RemoveOwningElementProcedure::operator()(ID id, Element* el) const {
    if (el->getOwnedComments().count(m_me->getID())) {
        el->getOwnedComments().remove(*m_me);
    }
}

void Comment::AddOwningElementProcedure::operator()(ID id, Element* el) const {
    if (!el->getOwnedComments().count(m_me->getID())) {
        el->getOwnedComments().add(*m_me);
    }
}

Comment::Comment() {
    m_owningElement.m_signature = &Comment::m_owningElement;
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

void Comment::setOwningElement(Element* el) {
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