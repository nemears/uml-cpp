#include "uml/redefinableElement.h"
#include "uml/behavior.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/deployment.h"
#include "uml/interface.h"

using namespace UML;

void RedefinableElement::referencingReleased(ID id) {
    m_redefinedElement.release(id);
    m_redefinitionContext.release(id);
}

void RedefinableElement::referenceReindexed(ID oldID, ID newID) {
    m_redefinedElement.reindex(oldID, newID);
    m_redefinitionContext.reindex(oldID, newID);
}

void RedefinableElement::reindexName(std::string oldName, std::string newName) {
    m_redefinedElement.reindexName(oldName, newName);
    m_redefinitionContext.reindexName(oldName, newName);
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