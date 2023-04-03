#include "uml/types/redefinableElement.h"
#include "uml/types/behavior.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/deployment.h"
#include "uml/types/interface.h"

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

}

RedefinableElement::~RedefinableElement() {

}

ReadOnlySet<RedefinableElement, RedefinableElement>& RedefinableElement::getRedefinedElements() {
    return m_redefinedElement;
}

ReadOnlySet<Classifier, RedefinableElement>& RedefinableElement::getRedefinitionContext() {
    return m_redefinitionContext;
}

bool RedefinableElement::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::REDEFINABLE_ELEMENT;
    }

    return ret;
}