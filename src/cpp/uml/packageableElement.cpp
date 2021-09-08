#include "uml/packageableElement.h"
#include "uml/package.h"

using namespace UML;

void PackageableElement::RemoveOwningPackageProcedure::operator()(Package* el) const {
    if (el->getPackagedElements().count(m_me->getID())) {
        el->getPackagedElements().remove(*m_me);
    }
}

void PackageableElement::AddOwningPackageProcedure::operator()(Package* el) const {
    if (!el->getPackagedElements().count(m_me->getID())) {
        el->getPackagedElements().add(*m_me);
    }
}

PackageableElement::PackageableElement() {
    m_owningPackage.m_signature = &PackageableElement::m_owningPackage;
    m_owningPackage.m_removeProcedures.push_back(new RemoveOwningPackageProcedure(this));
    m_owningPackage.m_addProcedures.push_back(new AddOwningPackageProcedure(this));
}

PackageableElement::PackageableElement(const PackageableElement& el) : NamedElement(el) {
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

bool PackageableElement::hasOwningPackage() const {
    return m_owningPackage.has();
}

void PackageableElement::setOwningPackage(Package* package) {
    m_owningPackage.set(package);
}

void PackageableElement::setOwningPackage(Package& package) {
    m_owningPackage.set(package);
}

ElementType PackageableElement::getElementType() const {
    return ElementType::PACKAGEABLE_ELEMENT;
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

void PackageableElement::restoreReleased(ID id, Element* released) {
    Element::restoreReleased(id, released);
}

void PackageableElement::referencingReleased(ID id) {
    Element::referencingReleased(id);
    if (m_owningPackage.id() == id) {
        m_owningPackage.release();
    }
}