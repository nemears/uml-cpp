#include "uml/types/extension.h"
#include "uml/types/extensionEnd.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<ExtensionEnd, Extension>& Extension::getOwnedEndSingleton() {
    return m_ownedEnd;
}

Extension::Extension() : Element(ElementType::EXTENSION) {
    m_ownedEnd.redefines(m_ownedEnds);
}

Extension::~Extension() {
    
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