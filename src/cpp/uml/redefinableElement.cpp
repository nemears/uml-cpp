#include "uml/redefinableElement.h"
#include "uml/uml-stable.h"

using namespace UML;

void RedefinableElement::referencingReleased(ID id) {
    m_redefinedElement.release(id);
    m_redefinitionContext.release(id);
}

void RedefinableElement::referenceReindexed(ID oldID, ID newID) {
    m_redefinedElement.reindex(oldID, newID);
    m_redefinitionContext.reindex(oldID, newID);
}

void RedefinableElement::restoreReferences() {
    // m_redefinedElement.restoreReferences();
    // m_redefinitionContext.restoreReferences();
}

void RedefinableElement::referenceErased(ID id) {
    m_redefinedElement.eraseElement(id);
    m_redefinitionContext.eraseElement(id);
}

void RedefinableElement::init() {
    m_redefinedElement.m_readOnly = true;
    m_redefinedElement.m_signature = &RedefinableElement::getRedefinedElements;
    m_redefinitionContext.m_readOnly = true;
    m_redefinitionContext.m_signature = &RedefinableElement::getRedefinitionContext;
}

RedefinableElement::RedefinableElement() : Element(ElementType::REDEFINABLE_ELEMENT) {
    init();
}

RedefinableElement::~RedefinableElement() {

}

RedefinableElement::RedefinableElement(const RedefinableElement& el) : Element(el, ElementType::REDEFINABLE_ELEMENT) {
    // abstract
}

Set<RedefinableElement, RedefinableElement>& RedefinableElement::getRedefinedElements() {
    return m_redefinedElement;
}

Set<Classifier, RedefinableElement>& RedefinableElement::getRedefinitionContext() {
    return m_redefinitionContext;
}

bool RedefinableElement::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::REDEFINABLE_ELEMENT;
    }

    return ret;
}