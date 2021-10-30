#include "uml/packageableElement.h"
#include "uml/package.h"

using namespace UML;

void PackageableElement::RemoveOwningPackageProcedure::operator()(Package* el) const {
    if (el->getPackagedElements().count(m_me->getID())) {
        el->getPackagedElements().remove(*m_me);
    }
    if (m_me->getNamespaceID() == el->getID()) {
        m_me->setNamespace(0);
    }
}

void PackageableElement::AddOwningPackageProcedure::operator()(Package* el) const {
    if (!el->getPackagedElements().count(m_me->getID())) {
        el->getPackagedElements().add(*m_me);
    }
    if (m_me->getNamespaceID() != el->getID()) {
        m_me->setNamespace(el);
    }
}

void PackageableElement::AddOwningPackageProcedure::operator()(ID id) const {
    if (m_me->getNamespaceID() != id) {
        m_me->m_namespace.setByID(id);
    }
}

void PackageableElement::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    ParameterableElement::referenceReindexed(oldID, newID);
    m_owningPackage.reindex(oldID, newID);
}

void PackageableElement::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    ParameterableElement::referencingReleased(id);
    m_owningPackage.release(id);
}

void PackageableElement::restoreReferences() {
    NamedElement::restoreReferences();
    ParameterableElement::restoreReferences();
    m_owningPackage.restoreReference();
}

void PackageableElement::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    ParameterableElement::referenceErased(id);
    m_owningPackage.elementErased(id);
}

PackageableElement::PackageableElement() : Element(ElementType::PACKAGEABLE_ELEMENT) {
    m_owningPackage.m_signature = &PackageableElement::m_owningPackage;
    m_owningPackage.m_removeProcedures.push_back(new RemoveOwningPackageProcedure(this));
    m_owningPackage.m_addProcedures.push_back(new AddOwningPackageProcedure(this));
}

PackageableElement::PackageableElement(const PackageableElement& el) : 
NamedElement(el), 
Element(el, ElementType::PACKAGEABLE_ELEMENT) {
    m_owningPackage = el.m_owningPackage;
    m_owningPackage.m_me = this;
    m_owningPackage.m_removeProcedures.clear();
    m_owningPackage.m_addProcedures.clear();
    m_owningPackage.m_removeProcedures.push_back(new RemoveOwningPackageProcedure(this));
    m_owningPackage.m_addProcedures.push_back(new AddOwningPackageProcedure(this));
}

Package* PackageableElement::getOwningPackage() {
    return m_owningPackage.get();
}

Package& PackageableElement::getOwningPackageRef() {
    return m_owningPackage.getRef();
}

ID PackageableElement::getOwningPackageID() const {
    return m_owningPackage.id();
}

bool PackageableElement::hasOwningPackage() const {
    return m_owningPackage.has();
}

void PackageableElement::setOwningPackage(Package* package) {
    m_owningPackage.set(package);
}

void PackageableElement::setOwningPackage(Package& package) {
    m_owningPackage.set(package);
}

bool PackageableElement::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = ParameterableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PACKAGEABLE_ELEMENT;
    }

    return ret;
}