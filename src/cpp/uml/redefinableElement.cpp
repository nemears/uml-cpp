#include "uml/redefinableElement.h"
#include "uml/classifier.h"

using namespace UML;

// void RedefinableElement::AddRedefinedElementFunctor::operator()(RedefinableElement& el) const {
//     el.setReference(m_el);
//     updateCopiedSequenceAddedTo(el, &RedefinableElement::getRedefinedElements);
// }

// void RedefinableElement::RemoveRedefinedElementFunctor::operator()(RedefinableElement& el) const {
//     el.removeReference(m_el->getID());
//     updateCopiedSequenceRemovedFrom(el, &RedefinableElement::getRedefinedElements);
// }

// void RedefinableElement::AddRedefinitionContextFunctor::operator()(Classifier& el) const {
//     updateCopiedSequenceAddedTo(el, &RedefinableElement::getRedefinitionContext);
// }

// void RedefinableElement::RemoveRedefinitionContextFunctor::operator()(Classifier& el) const {
//     updateCopiedSequenceRemovedFrom(el, &RedefinableElement::getRedefinitionContext);
// }

void RedefinableElement::referencingReleased(ID id) {
    // m_redefinedElement.elementReleased(id, &RedefinableElement::getRedefinedElements);
    // m_redefinitionContext.elementReleased(id, &RedefinableElement::getRedefinitionContext);
}

void RedefinableElement::referenceReindexed(ID oldID, ID newID) {
    // m_redefinedElement.reindex(oldID, newID, &RedefinableElement::getRedefinedElements);
    // m_redefinitionContext.reindex(oldID, newID, &RedefinableElement::getRedefinitionContext);
}

void RedefinableElement::restoreReferences() {
    // m_redefinedElement.restoreReferences();
    // m_redefinitionContext.restoreReferences();
}

void RedefinableElement::referenceErased(ID id) {
    // m_redefinedElement.elementErased(id);
    // m_redefinitionContext.elementErased(id);
}

RedefinableElement::RedefinableElement() : Element(ElementType::REDEFINABLE_ELEMENT) {

}

RedefinableElement::~RedefinableElement() {

}

RedefinableElement::RedefinableElement(const RedefinableElement& el) : Element(el, ElementType::REDEFINABLE_ELEMENT) {
    
}

// Sequence<RedefinableElement>& RedefinableElement::getRedefinedElements() {
//     return m_redefinedElement;
// }

// Sequence<Classifier>& RedefinableElement::getRedefinitionContext() {
//     return m_redefinitionContext;
// }

bool RedefinableElement::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::REDEFINABLE_ELEMENT;
    }

    return ret;
}