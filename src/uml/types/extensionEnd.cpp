#include "uml/uml-stable.h"

using namespace UML;

Singleton<Stereotype, ExtensionEnd>& ExtensionEnd::getTypeSingleton() {
    return m_extensionType;
}

ExtensionEnd::ExtensionEnd() : Element(ElementType::EXTENSION_END) {
    m_extensionType.redefines(m_type);
}

ExtensionEnd::~ExtensionEnd() {
    
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

bool ExtensionEnd::is(ElementType eType) const {
    bool ret = Property::is(eType);

    if (!ret) {
        ret = eType == ElementType::EXTENSION_END;
    }

    return ret;
}
