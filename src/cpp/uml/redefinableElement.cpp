#include "uml/redefinableElement.h"

using namespace UML;

RedefinableElement::RedefinableElement() {
    m_redefinedElement = new Sequence<RedefinableElement>;
    m_redefinitionContext = new Sequence<Classifier>;
}

RedefinableElement::~RedefinableElement() {
    delete m_redefinedElement;
    delete m_redefinitionContext;
}

RedefinableElement::RedefinableElement(const RedefinableElement& el) {
    m_redefinedElement = new Sequence<RedefinableElement>(*el.m_redefinedElement);
    m_redefinitionContext = new Sequence<Classifier>(*el.m_redefinitionContext);
}

Sequence<RedefinableElement>& RedefinableElement::getRedefinedElements() {
    return *m_redefinedElement;
}

Sequence<Classifier>& RedefinableElement::getRedefinitionContext() {
    return *m_redefinitionContext;
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