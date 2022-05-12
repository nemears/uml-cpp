#include "uml/constraint.h"
#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/valueSpecification.h"
#include "uml/umlPtr.h"

using namespace UML;

Set<Namespace, Constraint>& Constraint::getContextSingleton() {
    return m_context;
}

void Constraint::referencingReleased(ID id) {
    PackageableElement::referencingReleased(id);
    m_constrainedElements.release(id);
}

void Constraint::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    m_constrainedElements.reindex(oldID, newID);
}

void Constraint::reindexName(ID id, std::string newName) {
    PackageableElement::reindexName(id, newName);
    m_constrainedElements.reindexName(id, newName);
}

void Constraint::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    m_constrainedElements.eraseElement(id);
}

void Constraint::init() {
    m_context.subsets(m_namespace);
    m_context.opposite(&Namespace::getOwnedRules);
    m_specifications.subsets(*m_ownedElements);
}

Constraint::Constraint() : Element(ElementType::CONSTRAINT) {
    init();
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

Set<ValueSpecification, Constraint>& Constraint::getSpecifications() {
    return m_specifications;
}

bool Constraint::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CONSTRAINT;
    }

    return ret;
}