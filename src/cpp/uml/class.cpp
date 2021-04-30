#include "uml/class.h"
#include "uml/operation.h"

using namespace UML;

void Class::AddOperationFunctor::operator()(Element& el) const {
    if (dynamic_cast<Operation&>(el).getClass() != m_el) {
        dynamic_cast<Operation&>(el).setClass(dynamic_cast<Class*>(m_el));
    }

    if (dynamic_cast<Operation&>(el).getNamespace() != m_el) {
        dynamic_cast<Operation&>(el).setNamespace(dynamic_cast<Class*>(m_el));
    }
}

void Class::RemoveOperationFunctor::operator()(Element& el) const {
    if (dynamic_cast<Operation&>(el).getClass() == m_el) {
        dynamic_cast<Operation&>(el).setClass(0);
    }

    if (dynamic_cast<Class*>(m_el)->getFeatures().count(el.getID())) {
        dynamic_cast<Class*>(m_el)->getFeatures().remove(dynamic_cast<Operation&>(el));
    }
}

Class::Class() {
    m_operations = new Sequence<Operation>;
    m_operations->addProcedures.push_back(new AddOperationFunctor(this));
    m_operations->removeProcedures.push_back(new RemoveOperationFunctor(this));
}

Class::~Class() {
    delete m_operations;
}

Sequence<Operation>& Class::getOperations() {
    return *m_operations;
}

ElementType Class::getElementType() const {
    return ElementType::CLASS;
}

bool Class::isSubClassOf(ElementType eType) {
    bool ret = StructuredClassifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CLASS;
    }
    
    return ret;
}