#include "uml/elementImport.h"
#include "uml/behavior.h"
#include "uml/umlPtr.h"
#include "uml/property.h"
#include "uml/package.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/generalizationSet.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"
#include "uml/setReferenceFunctor.h"

using namespace UML;

void ElementImport::AddImportedElementFunctor::operator()(Element& el) const {
    if (m_el.as<ElementImport>().getImportingNamespace()) {
        m_el.as<ElementImport>().getImportingNamespace()->getImportedMembers().addReadOnly(el.as<PackageableElement>());
    }
}

void ElementImport::RemoveImportedElementFunctor::operator()(Element& el) const {
    if (m_el.as<ElementImport>().getImportingNamespace()) {
        m_el.as<ElementImport>().getImportingNamespace()->getImportedMembers().removeReadOnly(el.getID());
    }
}

Set<PackageableElement, ElementImport>& ElementImport::getImportedElementSingleton() {
    return m_importedElement;
}

Set<Namespace, ElementImport>& ElementImport::getImportingNamespaceSingleton() {
    return m_importingNamespace;
}

void ElementImport::init() {
    m_importedElement.subsets(m_targets);
    m_importedElement.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_importedElement.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_importedElement.m_addFunctors.insert(new AddImportedElementFunctor(this));
    m_importedElement.m_removeFunctors.insert(new RemoveImportedElementFunctor(this));
    m_importingNamespace.subsets(*m_owner);
    m_importingNamespace.subsets(m_sources);
    m_importingNamespace.opposite(&Namespace::getElementImports);
}

ElementImport::ElementImport() : Element(ElementType::ELEMENT_IMPORT) {
    init();
}

ElementImport::~ElementImport() {

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

bool ElementImport::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ELEMENT_IMPORT;
    }

    return ret;
}