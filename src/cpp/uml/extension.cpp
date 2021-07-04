#include "uml/extension.h"

using namespace UML;

Extension::Extension() {
    m_metaClass = ElementType::ELEMENT;
}

Extension::Extension(const Extension& extension) {
    m_metaClass = extension.m_metaClass;
}

Extension::~Extension() {

}

void Extension::setMetaClass(ElementType metaClass) {
    m_metaClass = metaClass;
}

ElementType Extension::getMetaClass() {
    return m_metaClass;
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