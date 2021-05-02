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

void Class::ClassAddOwnedAttributeFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getClass() != m_el) {
        dynamic_cast<Property&>(el).setClass(dynamic_cast<Class*>(m_el));
    }
}

void Class::ClassRemoveOwnedAttributeFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getClass() == m_el) {
        dynamic_cast<Property&>(el).setClass(0);
    }
}

Class::Class() {
    m_operations.addProcedures.push_back(new AddOperationFunctor(this));
    m_operations.removeProcedures.push_back(new RemoveOperationFunctor(this));
    m_ownedAttributes.addProcedures.push_back(new ClassAddOwnedAttributeFunctor(this));
    m_ownedAttributes.removeProcedures.push_back(new ClassRemoveOwnedAttributeFunctor(this));
}

Class::~Class() {
    
}

Class::Class(const Class& clazz) : StructuredClassifier(clazz) , PackageableElement(clazz), NamedElement(clazz), Element(clazz) {
    m_operations = clazz.m_operations;
    m_operations.addProcedures.clear();
    m_operations.addProcedures.push_back(new AddOperationFunctor(this));
    m_operations.removeProcedures.clear();
    m_operations.removeProcedures.push_back(new RemoveOperationFunctor(this));
    m_ownedAttributes.addProcedures.push_back(new ClassAddOwnedAttributeFunctor(this));
    m_ownedAttributes.removeProcedures.push_back(new ClassRemoveOwnedAttributeFunctor(this));
}

Sequence<Operation>& Class::getOperations() {
    return m_operations;
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