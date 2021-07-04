#include "uml/extensionEnd.h"
#include "uml/extension.h"

using namespace UML;

ExtensionEnd::ExtensionEnd() {
    m_extensionPtr = 0;
}

ExtensionEnd::ExtensionEnd(const ExtensionEnd& end) {
    m_extensionID = end.m_extensionID;
    if (!m_manager) {
        m_extensionPtr = end.m_extensionPtr;
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