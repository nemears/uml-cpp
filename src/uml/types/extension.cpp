#include "uml/set/singleton.h"
#include "uml/uml-stable.h"

using namespace UML;

void Extension::MemberEndPolicy::elementAdded(Property& el, Extension& me) {
    // TODO validate that el is owned by a meta-class
    if (el.getType() && el.getType().loaded() && me.m_ownedEnd.get().id() != el.getID()) {
        me.m_metaClass.set(el.getType()->as<Class>());
    }
}

void Extension::MemberEndPolicy::elementRemoved(Property& el, Extension& me) {
    if (el.getType() && me.m_ownedEnd.get().id() != el.getID()) {
        me.m_metaClass.set(0);
    }
}

Singleton<ExtensionEnd, Extension>& Extension::getOwnedEndSingleton() {
    return m_ownedEnd;
}

void Extension::restoreReferences() {
    Association::restoreReferences();
    for (PropertyPtr end : m_extensionMemberEnds.ptrs()) {
        if (getOwnedEnd().id() == end.id()) {
            continue;
        }
        if (end->getType() && m_metaClass.get().id() != end->getType().id()) {
            m_metaClass.set(end->getType()->as<Class>());
        }
    }
}

Extension::Extension() : Element(ElementType::EXTENSION) {
    m_ownedEnd.redefines(m_ownedEnds);
    m_extensionMemberEnds.redefines(m_memberEnds);
}

Extension::~Extension() {
    
}

ExtensionEndPtr Extension::getOwnedEnd() const {
    return m_ownedEnd.get();
}

void Extension::setOwnedEnd(ExtensionEndPtr end) {
    m_ownedEnd.set(end);
}

void Extension::setOwnedEnd(ExtensionEnd& end) {
    m_ownedEnd.set(end);
}

void Extension::setOwnedEnd(ID id) {
    m_ownedEnd.set(id);
}

ClassPtr Extension::getMetaClass() const {
    return m_metaClass.get();
}

bool Extension::is(ElementType eType) const {
    bool ret = Association::is(eType);

    if (!ret) {
        ret = eType == ElementType::EXTENSION;
    }

    return ret;
}
