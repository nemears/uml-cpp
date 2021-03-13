#include "uml/class.h"
#include "uml/operation.h"

using namespace UML;

Class::Class() {
    m_operations = new Sequence<Operation>;
}

Class::~Class() {
    delete m_operations;
}

Sequence<Operation>& Class::getOperations() {
    return *m_operations;
}

ElementType Class::getElementType() {
    return ElementType::CLASS;
}