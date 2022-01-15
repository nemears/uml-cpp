#include "uml/extensionEnd.h"
#include "uml/extension.h"
#include "uml/uml-stable.h"

using namespace UML;

void ExtensionEnd::referencingReleased(ID id) {
    Property::referencingReleased(id);
    m_extensionType.release(id);
}

void ExtensionEnd::referenceReindexed(ID oldID, ID newID) {
    Property::referenceReindexed(oldID, newID);
    m_extensionType.reindex(oldID, newID);
}

void ExtensionEnd::referenceErased(ID id) {
    Property::referenceErased(id);
    m_extensionType.eraseElement(id);
}

Set<Stereotype, ExtensionEnd>& ExtensionEnd::getTypeSingleton() {
    return m_extensionType;
}

void ExtensionEnd::init() {
    m_extensionType.redefines(m_type);
    m_extensionType.m_signature = &ExtensionEnd::getTypeSingleton;
}

void ExtensionEnd::copy(const ExtensionEnd& rhs) {
    m_extensionType = rhs.m_extensionType;
}

ExtensionEnd::ExtensionEnd() : Element(ElementType::EXTENSION_END) {
    init();
}

ExtensionEnd::ExtensionEnd(const ExtensionEnd& rhs) : Element(rhs, ElementType::EXTENSION_END) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    TypedElement::copy(rhs);
    MultiplicityElement::copy(rhs);
    RedefinableElement::copy(rhs);
    Feature::copy(rhs);
    Property::copy(rhs);
    copy(rhs);
}

ExtensionEnd::~ExtensionEnd() {

}

Stereotype* ExtensionEnd::getType() {
    return m_extensionType.get();
}

Stereotype& ExtensionEnd::getTypeRef() {
    return m_extensionType.getRef();
}

bool ExtensionEnd::hasType() const {
    return m_extensionType.has();
}

void ExtensionEnd::setType(Stereotype* type) {
    m_extensionType.set(type);
}

void ExtensionEnd::setType(Stereotype& type) {
    m_extensionType.set(type);
}

bool ExtensionEnd::isSubClassOf(ElementType eType) const {
    bool ret = Property::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::EXTENSION_END;
    }

    return ret;
}