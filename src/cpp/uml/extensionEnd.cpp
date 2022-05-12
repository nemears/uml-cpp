#include "uml/extensionEnd.h"
#include "uml/extension.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

Set<Stereotype, ExtensionEnd>& ExtensionEnd::getTypeSingleton() {
    return m_extensionType;
}

void ExtensionEnd::init() {
    m_extensionType.redefines(m_type);
}

ExtensionEnd::ExtensionEnd() : Element(ElementType::EXTENSION_END) {
    init();
}

ExtensionEnd::~ExtensionEnd() {
    mountAndRelease();
}

TypePtr ExtensionEnd::getType() const {
    return m_extensionType.get();
}

void ExtensionEnd::setType(Stereotype* type) {
    m_extensionType.set(type);
}

void ExtensionEnd::setType(Stereotype& type) {
    m_extensionType.set(type);
}

bool ExtensionEnd::isSubClassOf(ElementType eType) const {
    bool ret = Property::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::EXTENSION_END;
    }

    return ret;
}