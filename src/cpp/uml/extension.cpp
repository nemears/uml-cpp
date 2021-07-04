#include "uml/extension.h"

using namespace UML;

Extension::Extension() {

}

Extension::Extension(const Extension& extension) {

}

Extension::~Extension() {

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