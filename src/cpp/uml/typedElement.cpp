#include "uml/typedElement.h"
#include "uml/umlManager.h"
#include "uml/type.h"

using namespace UML;

void TypedElement::RemoveTypeProcedure::operator()(Type* el) const {
    if (el->m_node) {
        el->removeReference(m_me->getID());
    }
}

void TypedElement::AddTypeProcedure::operator()(Type* el) const {
    if (el->m_node) {
        el->setReference(m_me);
    }
}

TypedElement::TypedElement() {
    m_type.m_signature = &TypedElement::m_type;
    m_type.m_removeProcedures.push_back(new RemoveTypeProcedure(this));
    m_type.m_addProcedures.push_back(new AddTypeProcedure(this));
}

TypedElement::TypedElement(const TypedElement& el) {
    m_type = el.m_type;
    m_type.m_me = this;
    m_type.m_removeProcedures.clear();
    m_type.m_addProcedures.clear();
    m_type.m_removeProcedures.push_back(new RemoveTypeProcedure(this));
    m_type.m_addProcedures.push_back(new AddTypeProcedure(this));
}

Type* TypedElement::getType() {
    return m_type.get();
}

Type& TypedElement::getTypeRef() {
    return m_type.getRef();
}

bool TypedElement::hasType() const {
    return m_type.has();
}

void TypedElement::setType(Type* type) {
    m_type.set(type);
}

void TypedElement::setType(Type& type) {
    m_type.set(type);
}

ElementType TypedElement::getElementType() const {
    return ElementType::TYPED_ELEMENT;
}

bool TypedElement::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TYPED_ELEMENT;
    }

    return ret;
}

void TypedElement::restoreReleased(ID id, Element* el) {
    /** TODO: anything? **/
}

void TypedElement::referencingReleased(ID id) {
    if (m_type.id() == id) {
        m_type.release();
    }
}