#include "uml/extension.h"
#include "uml/extensionEnd.h"
#include "uml/uml-stable.h"

using namespace UML;

void Extension::referencingReleased(ID id) {
    Association::referencingReleased(id);
    m_ownedEnd.release(id);
}

void Extension::referenceReindexed(ID oldID, ID newID) {
    Association::referenceReindexed(oldID, newID);
    m_ownedEnd.reindex(oldID, newID);
}

void Extension::referenceErased(ID id) {
    Association::referenceErased(id);
    m_ownedEnd.eraseElement(id);
}

Set<ExtensionEnd, Extension>& Extension::getOwnedEndSingleton() {
    return m_ownedEnd;
}

void Extension::init() {
    m_ownedEnd.redefines(m_ownedEnds);
    m_ownedEnd.m_signature = &Extension::getOwnedEndSingleton;
}

void Extension::copy(const Extension& rhs) {
    m_ownedEnd = rhs.m_ownedEnd;
    m_metaClass = rhs.m_metaClass;
}

Extension::Extension() : Element(ElementType::EXTENSION) {
    init();
}

Extension::Extension(const Extension& rhs) : Element(rhs, ElementType::EXTENSION) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    TemplateableElement::copy(rhs);
    Classifier::copy(rhs);
    Relationship::copy(rhs);
    Association::copy(rhs);
    copy(rhs);
}

Extension::~Extension() {

}

void Extension::setMetaClass(ElementType metaClass) {
    m_metaClass = metaClass;
}

ElementType Extension::getMetaClass() {
    return m_metaClass;
}

ExtensionEnd* Extension::getOwnedEnd() {
    return m_ownedEnd.get();
}

ExtensionEnd& Extension::getOwnedEndRef() {
    return m_ownedEnd.getRef();
}

ID Extension::getOwnedEndID() const {
    return m_ownedEnd.id();
}

bool Extension::hasOwnedEnd() const {
    return m_ownedEnd.has();
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