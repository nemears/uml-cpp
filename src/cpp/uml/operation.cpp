#include "uml/operation.h"
#include "uml/class.h"

using namespace UML;

void Operation::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    if (m_class) {
        m_class->getOperations().reindex(oldID, newID);
    }
    Element::reindexID(oldID, newID);
}

void Operation::reindexName(string oldName, string newName) {
    if (m_class) {
        m_class->getOperations().reindex(m_id, oldName, newName);
    }
    NamedElement::reindexName(oldName, newName);
}

Operation::Operation() {
    m_type = 0;
    m_methods = new Sequence<Behavior>;
    m_parameters = new Sequence<Parameter>;
    m_class = 0;
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

Class* Operation::getClass() {
    return m_class;
}

void Operation::setClass(Class* clazz) {
    m_class = clazz;
}

ElementType Operation::getElementType(){
    return ElementType::OPERATION;
}