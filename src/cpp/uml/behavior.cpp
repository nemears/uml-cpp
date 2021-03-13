#include "uml/behavior.h"

using namespace UML;

Behavior::Behavior() {
    m_parameters = new Sequence<Parameter>();
}

Behavior::~Behavior() {
    delete m_parameters;
}

Sequence<Parameter>& Behavior::getParameters() {
    return *m_parameters;
}

ElementType Behavior::getElementType() {
    return ElementType::BEHAVIOR;
}