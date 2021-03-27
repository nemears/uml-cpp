#include "uml/behavior.h"
#include "uml/operation.h"

using namespace UML;

void Behavior::AddParameterFunctor::operator()(Element& el) const {
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().add(el);
    }

    if (dynamic_cast<Behavior*>(m_el)->getSpecification()) {
        dynamic_cast<Parameter&>(el).setOperation(dynamic_cast<Behavior*>(m_el)->getSpecification());
    }
}

Behavior::Behavior() {
    m_parameters = new Sequence<Parameter>();
    m_parameters->addProcedures.push_back(new AddParameterFunctor(this));
    m_specification = 0;
}

Behavior::~Behavior() {
    delete m_parameters;
}

Sequence<Parameter>& Behavior::getParameters() {
    return *m_parameters;
}

Operation* Behavior::getSpecification() {
    return m_specification;
}

void Behavior::setSpecification(Operation* specification) {
    m_specification = specification;
    if (!m_specification->getMethods().count(m_id)) {
        m_specification->getMethods().add(*this);
    }
}

ElementType Behavior::getElementType() {
    return ElementType::BEHAVIOR;
}