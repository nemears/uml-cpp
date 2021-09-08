#include "uml/redefinableElement.h"
#include "uml/classifier.h"

using namespace UML;

void RedefinableElement::AddRedefinedElementFunctor::operator()(RedefinableElement& el) const {
    updateCopiedSequenceAddedTo(el, &RedefinableElement::getRedefinedElements);
}

void RedefinableElement::RemoveRedefinedElementFunctor::operator()(RedefinableElement& el) const {
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

void RedefinableElement::restoreReleased(ID id, Element* released) {
    NamedElement::restoreReleased(id, released);
}

void RedefinableElement::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    m_redefinedElement.elementReleased(id);
    m_redefinitionContext.elementReleased(id);
}