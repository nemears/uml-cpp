#include "uml/comment.h"
#include "uml/sequence.h"
#include "uml/universalFunctions.h"

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
    if (m_manager) {
        return m_manager->get<>(this, m_owningElementID, &Comment::m_owningElementPtr);
    } else {
        return m_owningElementPtr;
    }
}

void Comment::setOwningElement(Element* el) {
    if (!isSameOrNull(m_owningElementID, el)) {
        if (m_manager) {
            m_manager->removeReference(m_id, m_owningElementID);
        }
        if (!m_owningElementPtr) {
            m_owningElementPtr = m_manager->get<>(this, m_owningElementID, &Comment::m_owningElementPtr);
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
        if (m_manager) {
            m_manager->setReference(m_id, m_owningElementID, this);
        }
        if (!el->getOwnedComments().count(m_id)) {
            el->getOwnedComments().add(*this);
        }
    }
    if (m_manager) {
        m_manager->updateCopiesSingleton<Comment>(this, m_ownerID, &Comment::m_owningElementID, &Comment::m_owningElementPtr);
    }
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