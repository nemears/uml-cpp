#include "uml/redefinableElement.h"

using namespace UML;

void RedefinableElement::setManager(UmlManager* manager) {
    m_redefinedElement.m_manager = manager;
    m_redefinitionContext.m_manager = manager;
}

RedefinableElement::RedefinableElement() {
}

RedefinableElement::~RedefinableElement() {

}

RedefinableElement::RedefinableElement(const RedefinableElement& el) {
    m_redefinedElement = el.m_redefinedElement;
    m_redefinitionContext = el.m_redefinitionContext;
}

Sequence<RedefinableElement>& RedefinableElement::getRedefinedElements() {
    return m_redefinedElement;
}

Sequence<Classifier>& RedefinableElement::getRedefinitionContext() {
    return m_redefinitionContext;
}

ElementType RedefinableElement::getElementType() const {
    return ElementType::REDEFINABLE_ELEMENT;
}

bool RedefinableElement::isSubClassOf(ElementType eType) {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::REDEFINABLE_ELEMENT;
    }

    return ret;
}