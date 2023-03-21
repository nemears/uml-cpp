#include "uml/types/constraint.h"
#include "uml/types/package.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/types/valueSpecification.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<Namespace, Constraint>& Constraint::getContextSingleton() {
    return m_context;
}

void Constraint::referenceReindexed(ID newID) {
    PackageableElement::referenceReindexed(newID);
    m_constrainedElements.reindex(newID);
}

void Constraint::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    m_constrainedElements.eraseElement(id);
}

Constraint::Constraint() : Element(ElementType::CONSTRAINT) {
    m_context.subsets(*m_namespace);
    m_context.opposite(&Namespace::getOwnedRules);
    m_specification.subsets(*m_ownedElements);
}

Constraint::~Constraint() {

}

NamespacePtr Constraint::getContext() const {
    return m_context.get();
}

void Constraint::setContext(Namespace* context) {
    m_context.set(context);
}

void Constraint::setContext(Namespace& context) {
    m_context.set(context);
}

void Constraint::setContext(NamespacePtr context) {
    m_context.set(context);
}

void Constraint::setContext(ID id) {
    m_context.set(id);
}

Set<Element, Constraint>& Constraint::getConstrainedElements() {
    return m_constrainedElements;
}

ValueSpecificationPtr Constraint::getSpecification() const {
    return m_specification.get();
}

void Constraint::setSpecification(ValueSpecification* specification) {
    m_specification.set(specification);
}

void Constraint::setSpecification(ValueSpecification& specification) {
    m_specification.set(specification);
}

void Constraint::setSpecification(ID id) {
    m_specification.set(id);
}


bool Constraint::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CONSTRAINT;
    }

    return ret;
}