#include "uml/extension.h"
#include "uml/extensionEnd.h"
#include "uml/universalFunctions.h"

using namespace UML;

Extension::Extension() {
    m_metaClass = ElementType::ELEMENT;
    m_ownedEndPtr = 0;
}

Extension::Extension(const Extension& extension) {
    m_metaClass = extension.m_metaClass;
    m_ownedEndID = extension.m_ownedEndID;
    if (!extension.m_manager) {
        m_ownedEndPtr = extension.m_ownedEndPtr;
    }
}

Extension::~Extension() {

}

void Extension::setMetaClass(ElementType metaClass) {
    m_metaClass = metaClass;
}

ElementType Extension::getMetaClass() {
    return m_metaClass;
}

ExtensionEnd* Extension::getOwnedEnd() {
    return universalGet<ExtensionEnd>(m_ownedEndID, m_ownedEndPtr, m_manager);
}

void Extension::setOwnedEnd(ExtensionEnd* end) {
    if (!isSameOrNull(m_ownedEndID, end)) {
        if (!m_ownedEndPtr) {
            m_ownedEndPtr = &m_manager->get<ExtensionEnd>(m_ownedEndID);
        }

        if (m_ownedEndPtr->getExtension() == this) {
            m_ownedEndPtr->setExtension(0);
        }

        if (m_ownedEnds.count(m_ownedEndID)) {
            m_ownedEnds.remove(*m_ownedEndPtr);
        }

        m_ownedEndID = ID::nullID();
        m_ownedEndPtr = 0;
    }

    if (end) {
        m_ownedEndID = end->getID();
    }

    if (!m_manager) {
        m_ownedEndPtr = end;
    }

    if (end) {
        if (!m_ownedEnds.count(end->getID())) {
            m_ownedEnds.add(*end);
        }
        if (end->getExtension() != this) {
            end->setExtension(this);
        }
    }
}

ElementType Extension::getElementType() const {
    return ElementType::EXTENSION;
}

bool Extension::isSubClassOf(ElementType eType) const {
    bool ret = Association::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::EXTENSION;
    }

    return ret;
}