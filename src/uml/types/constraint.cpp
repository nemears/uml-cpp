#include "uml/uml-stable.h"

using namespace UML;

Singleton<Namespace, Constraint>& Constraint::getContextSingleton() {
    return m_context;
}

void Constraint::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    eraseFromSet(id, m_constrainedElements);
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


bool Constraint::is(ElementType eType) const {
    bool ret = PackageableElement::is(eType);

    if (!ret) {
        ret = eType == ElementType::CONSTRAINT;
    }

    return ret;
}
