#include "uml/typedElement.h"
#include "uml/uml-stable.h"
#include "uml/setReferenceFunctor.h"

using namespace UML;

void TypedElement::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    m_type.release(id);
}

void TypedElement::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    m_type.reindex(oldID, newID);
}

void TypedElement::restoreReference(Element* el) {
    NamedElement::restoreReference(el);
    if (m_type.id() == el->getID()) {
        el->setReference(this);
    }
}

void TypedElement::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    m_type.eraseElement(id);
}

Set<Type, TypedElement>& TypedElement::getTypeSingleton() {
    return m_type;
}

void TypedElement::init() {
    m_type.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_type.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_type.m_signature = &TypedElement::getTypeSingleton;
}

void TypedElement::copy(const TypedElement& rhs) {
    m_type =  rhs.m_type;
}

TypedElement::TypedElement() : Element(ElementType::TYPED_ELEMENT) {
    init();
}

TypedElement::TypedElement(const TypedElement& el) : Element(ElementType::TYPE) {
    // abstract
}

TypedElement::~TypedElement() {
    
}

Type* TypedElement::getType() {
    return m_type.get();
}

Type& TypedElement::getTypeRef() {
    return m_type.getRef();
}

ID TypedElement::getTypeID() const {
    return m_type.id();
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