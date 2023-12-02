#include "uml/types/typedElement.h"
#include "uml/types/package.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void TypedElement::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    m_type.eraseElement(id);
}

TypedSet<Type, TypedElement>& TypedElement::getTypeSingleton() {
    return m_type;
}

TypedElement::TypedElement() : Element(ElementType::TYPED_ELEMENT) {
    
}

TypedElement::~TypedElement() {
    
}

TypePtr TypedElement::getType() const {
    return m_type.get();
}

void TypedElement::setType(Type* type) {
    m_type.set(type);
}

void TypedElement::setType(Type& type) {
    m_type.set(type);
}

void TypedElement::setType(ID id) {
    m_type.set(id);
}

bool TypedElement::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TYPED_ELEMENT;
    }

    return ret;
}