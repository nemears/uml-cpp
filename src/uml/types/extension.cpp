#include "uml/managers/abstractManager.h"
#include "uml/set/singleton.h"
#include "uml/types/association.h"
#include "uml/types/namedElement.h"
#include "uml/types/packageableElement.h"
#include "uml/types/parameterableElement.h"
#include "uml/uml-stable.h"

using namespace UML;

void Extension::MemberEndPolicy::elementAdded(Property& el, Extension& me) {
    // TODO validate that el is owned by a meta-class
    if (el.getType() && el.getType().loaded() && me.m_ownedEnd.get().id() != el.getID()) {
        me.addToReadonlySet(me.m_metaClass, el.getType()->as<Class>());
    }
}

void Extension::MemberEndPolicy::elementRemoved(Property& el, Extension& me) {
    if (el.getType() && me.m_ownedEnd.get().id() != el.getID()) {
        me.removeFromReadonlySet(me.m_metaClass, el);
    }
}

Singleton<ExtensionEnd, Extension>& Extension::getOwnedEndSingleton() {
    return m_ownedEnd;
}

Extension::Extension(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Association(elementType, manager)
{
    m_ownedEnd.redefines(m_ownedEnds);
    m_extensionMemberEnds.redefines(m_memberEnds);
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
