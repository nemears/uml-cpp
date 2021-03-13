#include "uml/operation.h"

using namespace UML;

Operation::Operation() {
    m_type = 0;
    m_methods = new Sequence<Behavior>;
    m_parameters = new Sequence<Parameter>;
}

Operation::~Operation() {
    delete m_methods;
    delete m_parameters;
}

Sequence<Behavior>& Operation::getMethods() {
    return *m_methods;
}

Sequence<Parameter>& Operation::getParameters() {
    return *m_parameters;
}

Type* Operation::getType() {
    return m_type;
}

void Operation::setType(Type* type) {
    m_type = type;
}

ElementType Operation::getElementType(){
    return ElementType::OPERATION;
}