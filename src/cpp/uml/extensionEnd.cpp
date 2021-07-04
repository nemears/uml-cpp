#include "uml/extensionEnd.h"

using namespace UML;

ExtensionEnd::ExtensionEnd() {

}

ExtensionEnd::ExtensionEnd(const ExtensionEnd& end) {

}

ExtensionEnd::~ExtensionEnd() {

}

ElementType ExtensionEnd::getElementType() const {
    return ElementType::EXTENSION_END;
}

bool ExtensionEnd::isSubClassOf(ElementType eType) const {
    bool ret = Property::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::EXTENSION_END;
    }

    return ret;
}