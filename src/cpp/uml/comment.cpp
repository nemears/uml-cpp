#include "uml/comment.h"
#include "uml/sequence.h"

using namespace std;
using namespace UML;

Comment::Comment() {
    m_owningElementPtr = 0;
}

string Comment::getBody() {
    return m_body;
}

void Comment::setBody(string body) {
    m_body = body;
}

Element* Comment::getOwningElement() {
    if (!m_owningElementID.isNull()) {
        if (!m_owningElementPtr) {
            m_owningElementPtr = &m_manager->get<>(m_owningElementID);
        }
        return m_owningElementPtr;
    }
    return 0;
}

void Comment::setOwningElement(Element* el) {
    if (!m_owningElementID.isNull()) {
        if (!m_owningElementPtr) {
            m_owningElementPtr = &m_manager->get<>(m_owningElementID);
        }

        if (m_owningElementPtr->getOwnedComments().count(m_id)) {
            m_owningElementPtr->getOwnedComments().remove(*this);
        }

        m_owningElementPtr = 0;
        m_owningElementID = ID::nullID();
    }

    if (el) {
        m_owningElementID = el->getID();
    }

    if (!m_manager) {
        m_owningElementPtr = el;
    }

    if (el) {
        if (!el->getOwnedComments().count(m_id)) {
            el->getOwnedComments().add(*this);
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