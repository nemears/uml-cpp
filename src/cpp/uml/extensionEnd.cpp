#include "uml/extensionEnd.h"
#include "uml/extension.h"

using namespace UML;

void ExtensionEnd::RemoveExtensionProcedure::operator()(Extension* el) const {
    if (el->hasOwnedEnd()) {
        el->setOwnedEnd(0);
    }
    if (m_me->hasOwningAssociation()) {
        m_me->setOwningAssociation(0);
    }
}

void ExtensionEnd::AddExtensionProcedure::operator()(Extension* el) const {
    if (el->hasOwnedEnd()) {
        if (el->getOwnedEndRef() != *m_me) {
            el->setOwnedEnd(m_me);
        }
    } else {
        el->setOwnedEnd(m_me);
    }
    if (m_me->hasOwningAssociation()) {
        if (m_me->getOwningAssociationRef() != *el) {
            m_me->setOwningAssociation(el);
        }
    } else {
        m_me->setOwningAssociation(el);
    }
}

void ExtensionEnd::RemoveExtensionTypeProcedure::operator()(Stereotype* el) const {
    if (m_me->TypedElement::hasType()) {
        m_me->TypedElement::setType(0);
    }
}

void ExtensionEnd::AddExtensionTypeProcedure::operator()(Stereotype* el) const {
    if (m_me->TypedElement::hasType()) {
        if (m_me->TypedElement::getTypeRef() != *m_me) {
            m_me->TypedElement::setType(el);
        }
    } else {
        m_me->TypedElement::setType(el);
    }
}

void ExtensionEnd::referencingReleased(ID id) {
    Property::referencingReleased(id);
    if (m_extension.id() == id) {
        m_extension.release();
    }
    if (m_extensionType.id() == id) {
        m_extensionType.release();
    }
}

void ExtensionEnd::referenceReindexed(ID oldID, ID newID) {
    Property::referenceReindexed(oldID, newID);
    if (m_extension.id() == oldID) {
        m_extension.reindex(oldID, newID);
    }
    if (m_extensionType.id() == oldID) {
        m_extensionType.reindex(oldID, newID);
    }
}

ExtensionEnd::ExtensionEnd() {
    m_extension.m_signature = &ExtensionEnd::m_extension;
    m_extension.m_removeProcedures.push_back(new RemoveExtensionProcedure(this));
    m_extension.m_addProcedures.push_back(new AddExtensionProcedure(this));
    m_extensionType.m_signature = &ExtensionEnd::m_extensionType;
    m_extensionType.m_addProcedures.push_back(new AddExtensionTypeProcedure(this));
    m_extensionType.m_removeProcedures.push_back(new RemoveExtensionTypeProcedure(this));
}

ExtensionEnd::ExtensionEnd(const ExtensionEnd& end) {
    m_extension = end.m_extension;
    m_extension.m_me = this;
    m_extension.m_addProcedures.clear();
    m_extension.m_removeProcedures.clear();
    m_extension.m_removeProcedures.push_back(new RemoveExtensionProcedure(this));
    m_extension.m_addProcedures.push_back(new AddExtensionProcedure(this));
    m_extensionType = end.m_extensionType;
    m_extensionType.m_me = this;
    m_extensionType.m_addProcedures.clear();
    m_extensionType.m_removeProcedures.clear();
    m_extensionType.m_addProcedures.push_back(new AddExtensionTypeProcedure(this));
    m_extensionType.m_removeProcedures.push_back(new RemoveExtensionTypeProcedure(this));
}

ExtensionEnd::~ExtensionEnd() {

}

Extension* ExtensionEnd::getExtension() {
    return m_extension.get();
}

Extension& ExtensionEnd::getExtensionRef() {
    return m_extension.getRef();
}

bool ExtensionEnd::hasExtension() const {
    return m_extension.has();
}

void ExtensionEnd::setExtension(Extension* extension) {
    m_extension.set(extension);
}

void ExtensionEnd::setExtension(Extension& extension) {
    m_extension.set(extension);
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

ElementType ExtensionEnd::getElementType() const {
    return ElementType::EXTENSION_END;
}

bool ExtensionEnd::isSubClassOf(ElementType eType) const {
    bool ret = Property::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::EXTENSION_END;
    }

    return ret;
}