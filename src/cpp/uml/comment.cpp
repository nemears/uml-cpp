#include "uml/comment.h"
#include "uml/sequence.h"

using namespace std;
using namespace UML;

Comment::Comment() {
    m_owningElement = 0;
}

string Comment::getBody() {
    return m_body;
}

void Comment::setBody(string body) {
    m_body = body;
}

Element* Comment::getOwningElement() {
    return m_owningElement;
}

void Comment::setOwningElement(Element* el) {
    if (m_owningElement) {
        if (m_owningElement != el) {
            if (m_owningElement->getOwnedComments().count(m_id)) {
                m_owningElement->getOwnedComments().remove(*this);
            }
        }
    }
    m_owningElement = el;
    if (m_owningElement) {
        if (!m_owningElement->getOwnedComments().count(m_id)) {
            m_owningElement->getOwnedComments().add(*this);
        }
    }
}

ElementType Comment::getElementType() const {
    return ElementType::COMMENT;
}

bool Comment::isSubClassOf(ElementType eType) {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::COMMENT;
    }

    return ret;
}