#include "uml/typedElement.h"
#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/setReferenceFunctor.h"
#include "uml/umlPtr.h"

using namespace UML;

void TypedElement::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    m_type.release(id);
}

void TypedElement::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    m_type.reindex(oldID, newID);
}

void TypedElement::reindexName(std::string oldName, std::string newName) {
    NamedElement::reindexName(oldName, newName);
    m_type.reindexName(oldName, newName);
}

void TypedElement::restoreReference(Element* el) {
    NamedElement::restoreReference(el);
    if (m_type.get().id() == el->getID()) {
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

TypedElement::TypedElement() : Element(ElementType::TYPED_ELEMENT) {
    init();
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