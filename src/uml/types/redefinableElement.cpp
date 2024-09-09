#include "uml/types/namedElement.h"
#include "uml/uml-stable.h"

using namespace UML;

RedefinableElement::RedefinableElement(std::size_t elementType, AbstractManager& manager) :
    Element(elementType, manager),
    NamedElement(elementType, manager)
{}

ReadOnlySet<RedefinableElement, RedefinableElement>& RedefinableElement::getRedefinedElements() {
    return m_redefinedElement;
}

ReadOnlySet<Classifier, RedefinableElement>& RedefinableElement::getRedefinitionContext() {
    return m_redefinitionContext;
}
