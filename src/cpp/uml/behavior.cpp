#include "uml/behavior.h"
#include "uml/operation.h"

using namespace UML;

Behavior::Behavior() {
    m_parameters = new Sequence<Parameter>();
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
}

ElementType Behavior::getElementType() {
    return ElementType::BEHAVIOR;
}