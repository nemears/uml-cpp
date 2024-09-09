#include "uml/uml-stable.h"

using namespace UML;

void ElementImport::ImportedElementPolicy::elementAdded(PackageableElement& el, ElementImport& me) {
    if (me.getImportingNamespace()) {
        me.addToReadonlySet(me.getImportingNamespace()->m_importedMembers, el);
    }
}

void ElementImport::ImportedElementPolicy::elementRemoved(PackageableElement& el, ElementImport& me) {
    if (me.getImportingNamespace()) {
        me.removeFromReadonlySet(me.getImportingNamespace()->m_importedMembers, el);
    }
}

Singleton<PackageableElement, ElementImport, ElementImport::ImportedElementPolicy>& ElementImport::getImportedElementSingleton() {
    return m_importedElement;
}

Singleton<Namespace, ElementImport>& ElementImport::getImportingNamespaceSingleton() {
    return m_importingNamespace;
}

ElementImport::ElementImport(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    DirectedRelationship(elementType, manager)
{
    m_importedElement.subsets(m_targets);
    m_importingNamespace.subsets(m_owner);
    m_importingNamespace.subsets(m_sources);
    m_importingNamespace.opposite(&Namespace::getElementImports);
}

PackageableElementPtr ElementImport::getImportedElement() const {
    return m_importedElement.get();
}

void ElementImport::setImportedElement(PackageableElement* importedElement) {
    m_importedElement.set(importedElement);
}

void ElementImport::setImportedElement(PackageableElement& importedElement) {
    m_importedElement.set(importedElement);
}

void ElementImport::setImportedElement(PackageableElementPtr importedElement) {
    m_importedElement.set(importedElement);
}

void ElementImport::setImportedElement(ID id) {
    m_importedElement.set(id);
}

NamespacePtr ElementImport::getImportingNamespace() const {
    return m_importingNamespace.get();
}

void ElementImport::setImportingNamespace(Namespace* importingNamespace) {
    m_importingNamespace.set(importingNamespace);
}

void ElementImport::setImportingNamespace(Namespace& importingNamespace) {
    m_importingNamespace.set(importingNamespace);
}

void ElementImport::setImportingNamespace(NamespacePtr importingNamespace) {
    m_importingNamespace.set(importingNamespace);
}

void ElementImport::setImportingNamespace(ID id) {
    m_importingNamespace.set(id);
}

std::string ElementImport::getAlias() const {
    return m_alias;
}
void ElementImport::setAlias(const std::string& alias) {
    m_alias = alias;
}

VisibilityKind ElementImport::getVisibility() const {
    return m_visibility;
}

void ElementImport::setVisibility(VisibilityKind visibility) {
    m_visibility = visibility;
}
