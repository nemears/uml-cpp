#include "uml/redefinableElement.h"
#include "uml/classifier.h"

using namespace UML;

void RedefinableElement::AddRedefinedElementFunctor::operator()(RedefinableElement& el) const {
    el.setReference(m_el);
    updateCopiedSequenceAddedTo(el, &RedefinableElement::getRedefinedElements);
}

void RedefinableElement::RemoveRedefinedElementFunctor::operator()(RedefinableElement& el) const {
    el.removeReference(m_el->getID());
    updateCopiedSequenceRemovedFrom(el, &RedefinableElement::getRedefinedElements);
}

void RedefinableElement::AddRedefinitionContextFunctor::operator()(Classifier& el) const {
    updateCopiedSequenceAddedTo(el, &RedefinableElement::getRedefinitionContext);
}

void RedefinableElement::RemoveRedefinitionContextFunctor::operator()(Classifier& el) const {
    updateCopiedSequenceRemovedFrom(el, &RedefinableElement::getRedefinitionContext);
}

void RedefinableElement::setManager(UmlManager* manager) {
    m_redefinedElement.m_manager = manager;
    m_redefinitionContext.m_manager = manager;
}

void RedefinableElement::referencingReleased(ID id) {
    m_redefinedElement.elementReleased(id, &RedefinableElement::getRedefinedElements);
    m_redefinitionContext.elementReleased(id, &RedefinableElement::getRedefinitionContext);
}

void RedefinableElement::referenceReindexed(ID oldID, ID newID) {
    m_redefinedElement.reindex(oldID, newID, &RedefinableElement::getRedefinedElements);
    m_redefinitionContext.reindex(oldID, newID, &RedefinableElement::getRedefinitionContext);
}

void RedefinableElement::restoreReferences() {
    m_redefinedElement.restoreReferences();
    m_redefinitionContext.restoreReferences();
}

void RedefinableElement::referenceErased(ID id) {
    m_redefinedElement.elementErased(id);
    m_redefinitionContext.elementErased(id);
}

RedefinableElement::RedefinableElement() {
    m_redefinedElement.addProcedures.push_back(new AddRedefinedElementFunctor(this));
    m_redefinedElement.removeProcedures.push_back(new RemoveRedefinedElementFunctor(this));
    m_redefinitionContext.addProcedures.push_back(new AddRedefinitionContextFunctor(this));
    m_redefinitionContext.removeProcedures.push_back(new RemoveRedefinitionContextFunctor(this));
}

RedefinableElement::~RedefinableElement() {

}

RedefinableElement::RedefinableElement(const RedefinableElement& el) {
    m_redefinedElement = el.m_redefinedElement;
    m_redefinedElement.m_el = this;
    m_redefinedElement.addProcedures.clear();
    m_redefinedElement.removeProcedures.clear();
    m_redefinedElement.addProcedures.push_back(new AddRedefinedElementFunctor(this));
    m_redefinedElement.removeProcedures.push_back(new RemoveRedefinedElementFunctor(this));
    m_redefinitionContext = el.m_redefinitionContext;
    m_redefinitionContext.m_el = this;
    m_redefinitionContext.addProcedures.clear();
    m_redefinitionContext.removeProcedures.clear();
    m_redefinitionContext.addProcedures.push_back(new AddRedefinitionContextFunctor(this));
    m_redefinitionContext.removeProcedures.push_back(new RemoveRedefinitionContextFunctor(this));
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

bool RedefinableElement::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::REDEFINABLE_ELEMENT;
    }

    return ret;
}