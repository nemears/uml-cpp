#include "uml/typedElement.h"
#include "uml/umlManager.h"

using namespace UML;

TypedElement::TypedElement() {
    m_typePtr = 0;
}

TypedElement::TypedElement(const TypedElement& el) {
    m_typeID = el.m_typeID;
    m_typePtr = el.m_typePtr;
}

Type* TypedElement::getType() {
    if (!m_typeID.isNull()) {
        if (!m_typePtr) {
            m_typePtr = &m_manager->get<Type>(m_typeID);
        }
        return m_typePtr;
    }
    return 0;
}

void TypedElement::setType(Type* type) {
    if (type) {
        m_typeID = type->getID();
    }

    if (!m_manager) {
        m_typePtr = type;
    }
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