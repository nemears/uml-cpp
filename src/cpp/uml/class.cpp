#include "uml/class.h"
#include "uml/operation.h"

using namespace UML;

void Class::AddOperationFunctor::operator()(Element& el) const {
    if (dynamic_cast<Operation&>(el).getClass() != m_el) {
        dynamic_cast<Operation&>(el).setClass(dynamic_cast<Class*>(m_el));
    }

    if (el.getOwner() != m_el) {
        el.setOwner(m_el);
    }
}

void Class::RemoveOperationFunctor::operator()(Element& el) const {
    if (dynamic_cast<Operation&>(el).getClass() == m_el) {
        dynamic_cast<Operation&>(el).setClass(0);
    }

    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().remove(el);
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

ElementType Class::getElementType() {
    return ElementType::CLASS;
}

bool Class::isSubClassOf(ElementType eType) {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::CLASS;
    }
    
    return ret;
}