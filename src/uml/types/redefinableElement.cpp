#include "uml/uml-stable.h"

using namespace UML;

void RedefinableElement::referenceErased(ID id) {
    eraseFromSet(id, m_redefinedElement);
    eraseFromSet(id, m_redefinitionContext);
}

RedefinableElement::RedefinableElement() : Element(ElementType::REDEFINABLE_ELEMENT) {

}

RedefinableElement::~RedefinableElement() {

}

ReadOnlySet<RedefinableElement, RedefinableElement>& RedefinableElement::getRedefinedElements() {
    return m_redefinedElement;
}

ReadOnlySet<Classifier, RedefinableElement>& RedefinableElement::getRedefinitionContext() {
    return m_redefinitionContext;
}

bool RedefinableElement::is(ElementType eType) const {
    bool ret = NamedElement::is(eType);

    if (!ret) {
        ret = eType == ElementType::REDEFINABLE_ELEMENT;
    }

    return ret;
}
