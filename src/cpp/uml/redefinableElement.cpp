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


void RedefinableElement::referenceReindexed(ID newID) {
    m_redefinedElement.reindex(newID);
    m_redefinitionContext.reindex(newID);
}

void RedefinableElement::referenceErased(ID id) {
    m_redefinedElement.eraseElement(id);
    m_redefinitionContext.eraseElement(id);
}

RedefinableElement::RedefinableElement() : Element(ElementType::REDEFINABLE_ELEMENT) {
    m_redefinedElement.m_readOnly = true;
    m_redefinitionContext.m_readOnly = true;
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