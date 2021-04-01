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

void Operation::AddMethodFunctor::operator()(Element& el) const {
    if (!dynamic_cast<Behavior&>(el).getSpecification()) {
        dynamic_cast<Behavior&>(el).setSpecification(dynamic_cast<Operation*>(m_el));
    }
}

Operation::Operation() {
    m_type = 0;
    m_methods = new Sequence<Behavior>;
    m_methods->addProcedures.push_back(new AddMethodFunctor(this));
    m_class = 0;
}

Operation::~Operation() {
    delete m_methods;
}

Sequence<Behavior>& Operation::getMethods() {
    return *m_methods;
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
    if (m_class) {
        if(m_class->getOperations().count(m_id)) {
            m_class->getOperations().remove(*this);
        }
    }
    m_class = clazz;
    if (m_class) {
        if (!m_class->getOperations().count(m_id)) {
            m_class->getOperations().add(*this);
        }
    }
}

ElementType Operation::getElementType(){
    return ElementType::OPERATION;
}