#include "uml/class.h"
#include "uml/operation.h"

using namespace UML;

void Class::AddOperationFunctor::operator()(Element& el) const {
    if (dynamic_cast<Operation&>(el).getClass() != m_el) {
        dynamic_cast<Operation&>(el).setClass(dynamic_cast<Class*>(m_el));
    }
}

Class::Class() {
    m_operations = new Sequence<Operation>;
    m_operations->addProcedures.push_back(new AddOperationFunctor(this));
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