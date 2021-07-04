#include "uml/extensionEnd.h"
#include "uml/extension.h"

using namespace UML;

ExtensionEnd::ExtensionEnd() {
    m_extensionPtr = 0;
    m_extensionTypePtr = 0;
}

ExtensionEnd::ExtensionEnd(const ExtensionEnd& end) {
    m_extensionID = end.m_extensionID;
    m_extensionTypeID = end.m_extensionTypeID;
    if (!end.m_manager) {
        m_extensionPtr = end.m_extensionPtr;
        m_extensionTypePtr = end.m_extensionTypePtr;
    }
}

ExtensionEnd::~ExtensionEnd() {

}

Extension* ExtensionEnd::getExtension() {
    if (!m_extensionID.isNull()) {
        if (!m_extensionPtr) {
            m_extensionPtr = &m_manager->get<Extension>(m_extensionID);
        }
        return m_extensionPtr;
    }
    return 0;
}

void ExtensionEnd::setExtension(Extension* extension) {
    if (!isSameOrNull(m_extensionID, extension)) {
        if (!m_extensionPtr) {
            m_extensionPtr = &m_manager->get<Extension>(m_extensionID);
        }

        Extension* temp = m_extensionPtr;

        if (getOwningAssociation() == m_extensionPtr) {
            setOwningAssociation(0);
        }

        m_extensionPtr = 0;
        m_extensionID = ID::nullID();

        if (temp->getOwnedEnd() == this) {
            temp->setOwnedEnd(0);
        }
    }

    if (extension) {
        m_extensionID = extension->getID();
    }

    if (!m_manager) {
        m_extensionPtr = extension;
    }

    if (extension) {
        if (getOwningAssociation() != extension) {
            setOwningAssociation(extension);
        }
        if (extension->getOwnedEnd() != this) {
            extension->setOwnedEnd(this);
        }
    }
}

Stereotype* ExtensionEnd::getType() {
    if (!m_extensionTypeID.isNull()) {
        if (!m_extensionTypePtr) {
            m_extensionTypePtr = &m_manager->get<Stereotype>(m_extensionTypeID);
        }
        return m_extensionTypePtr;
    }
    return 0;
}

void ExtensionEnd::setType(Stereotype* type) {
    if (!m_extensionTypeID.isNull()) {
        m_extensionTypeID = ID::nullID();
        m_extensionTypePtr = 0;
        Property::setType(0);
    }

    if (type) {
        m_extensionTypeID = type->getID();
    }

    if (!m_manager) {
        m_extensionTypePtr = type;
    }

    if (type) {
        Property::setType(type);
    }
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