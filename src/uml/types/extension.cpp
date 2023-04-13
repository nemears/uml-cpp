#include "uml/types/extension.h"
#include "uml/types/extensionEnd.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Extension::ExtensionAddMemberEndPolicy::apply(Property& el, Extension& me) {
    // TODO validate that el is owned by a meta-class
    if (el.getType() && el.getType().loaded() && me.m_ownedEnd.get().id() != el.getID()) {
        me.m_metaClass.set(el.getType()->as<Class>());
    }
}

void Extension::ExtensionRemoveMemberEndPolicy::apply(Property& el, Extension& me) {
    if (el.getType() && me.m_ownedEnd.get().id() != el.getID()) {
        me.m_metaClass.set(0);
    }
}

TypedSet<ExtensionEnd, Extension>& Extension::getOwnedEndSingleton() {
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

void Extension::setOwnedEnd(ExtensionEnd* end) {
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

bool Extension::isSubClassOf(ElementType eType) const {
    bool ret = Association::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::EXTENSION;
    }

    return ret;
}