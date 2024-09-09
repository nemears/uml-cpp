#include "uml/uml-stable.h"

using namespace UML;

Singleton<Namespace, Constraint>& Constraint::getContextSingleton() {
    return m_context;
}

Constraint::Constraint(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager)
{
    m_context.subsets(m_namespace);
    m_context.opposite(&Namespace::getOwnedRules);
    m_specification.subsets(m_ownedElements);
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
