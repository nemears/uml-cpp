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

void TypedElement::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    m_type.release();
}

void TypedElement::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    m_type.reindex(oldID, newID);
}

void TypedElement::restoreReferences() {
    NamedElement::restoreReferences();
    m_type.restoreReference();
}

void TypedElement::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    m_type.elementErased(id);
}

TypedElement::TypedElement() : Element(ElementType::TYPED_ELEMENT) {
    m_type.m_signature = &TypedElement::m_type;
    m_type.m_removeProcedures.push_back(new RemoveTypeProcedure(this));
    m_type.m_addProcedures.push_back(new AddTypeProcedure(this));
}

TypedElement::TypedElement(const TypedElement& el) : 
NamedElement(el),
Element(el, ElementType::TYPE) {
    m_type = el.m_type;
    m_type.m_me = this;
    m_type.m_removeProcedures.clear();
    m_type.m_addProcedures.clear();
    m_type.m_removeProcedures.push_back(new RemoveTypeProcedure(this));
    m_type.m_addProcedures.push_back(new AddTypeProcedure(this));
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

bool TypedElement::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TYPED_ELEMENT;
    }

    return ret;
}