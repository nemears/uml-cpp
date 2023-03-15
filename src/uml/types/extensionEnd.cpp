#include "uml/types/extensionEnd.h"
#include "uml/types/extension.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<Stereotype, ExtensionEnd>& ExtensionEnd::getTypeSingleton() {
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

bool ExtensionEnd::isSubClassOf(ElementType eType) const {
    bool ret = Property::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::EXTENSION_END;
    }

    return ret;
}