#include "uml/extension.h"
#include "uml/extensionEnd.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

Set<ExtensionEnd, Extension>& Extension::getOwnedEndSingleton() {
    return m_ownedEnd;
}

void Extension::init() {
    m_ownedEnd.redefines(m_ownedEnds);
}

Extension::Extension() : Element(ElementType::EXTENSION) {
    init();
}

Extension::~Extension() {
    mountAndRelease();
}

void Extension::setMetaClass(ElementType metaClass) {
    m_metaClass = metaClass;
}

ElementType Extension::getMetaClass() {
    return m_metaClass;
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

bool Extension::isSubClassOf(ElementType eType) const {
    bool ret = Association::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::EXTENSION;
    }

    return ret;
}