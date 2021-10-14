#include "uml/extension.h"
#include "uml/extensionEnd.h"

using namespace UML;

void Extension::RemoveOwnedEndProcedure::operator()(ExtensionEnd* el) const {
    if (el->hasExtension()&& !m_me->m_setFlag) {
        m_me->m_setFlag = true;
        el->setExtension(0);
        m_me->m_setFlag = false;
    }
    if (m_me->getOwnedEnds().count(el->getID())) {
        m_me->getOwnedEnds().remove(*el);
    }
}

void Extension::AddOwnedEndProcedure::operator()(ExtensionEnd* el) const {
    if (el->hasExtension()) {
        if (el->getExtensionRef() != *m_me) {
            el->setExtension(m_me);
        }
    }
    else {
        el->setExtension(m_me);
    }
    if (!m_me->getOwnedEnds().count(el->getID())) {
        m_me->getOwnedEnds().add(*el);
    }
}

void Extension::referencingReleased(ID id) {
    Association::referencingReleased(id);
    if (m_ownedEnd.id() == id) {
        m_ownedEnd.release();
    }
}

void Extension::referenceReindexed(ID oldID, ID newID) {
    Association::referenceReindexed(oldID, newID);
    if (m_ownedEnd.id() == oldID) {
        m_ownedEnd.reindex(oldID, newID);
    }
}

void Extension::referenceErased(ID id) {
    Association::referenceErased(id);
    m_ownedEnd.elementErased(id);
}

Extension::Extension() {
    m_metaClass = ElementType::ELEMENT;
    m_ownedEnd.m_signature = &Extension::m_ownedEnd;
    m_ownedEnd.m_addProcedures.push_back(new AddOwnedEndProcedure(this));
    m_ownedEnd.m_removeProcedures.push_back(new RemoveOwnedEndProcedure(this));
}

Extension::Extension(const Extension& extension) {
    m_metaClass = extension.m_metaClass;
    m_ownedEnd = extension.m_ownedEnd;
    m_ownedEnd.m_me = this;
    m_ownedEnd.m_addProcedures.clear();
    m_ownedEnd.m_removeProcedures.clear();
    m_ownedEnd.m_addProcedures.push_back(new AddOwnedEndProcedure(this));
    m_ownedEnd.m_removeProcedures.push_back(new RemoveOwnedEndProcedure(this));
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
    return m_ownedEnd.get();
}

ExtensionEnd& Extension::getOwnedEndRef() {
    return m_ownedEnd.getRef();
}

bool Extension::hasOwnedEnd() const {
    return m_ownedEnd.has();
}

void Extension::setOwnedEnd(ExtensionEnd* end) {
    m_ownedEnd.set(end);
}

void Extension::setOwnedEnd(ExtensionEnd& end) {
    m_ownedEnd.set(end);
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