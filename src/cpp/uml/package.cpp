#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/packageMerge.h"
#include "uml/profileApplication.h"

using namespace UML;

void Package::AddPackagedElementFunctor::operator()(PackageableElement& el) const {
    if (el.getOwningPackage() != m_el) {
        el.setOwningPackage(m_el);
    }

    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }

    updateCopiedSequenceAddedTo(el, &Package::getPackagedElements);
}

void Package::RemovePackagedElementFunctor::operator()(PackageableElement& el) const {
    if (el.getOwningPackage() == m_el) {
        el.setOwningPackage(0);
    }

    if (m_el->getMembers().count(el.getID())) {
        m_el->getMembers().remove(el);
    }

    updateCopiedSequenceRemovedFrom(el, &Package::getPackagedElements);
}

void Package::AddPackageMergeFunctor::operator()(PackageMerge& el) const {
    if (el.getReceivingPackage() != m_el) {
        el.setReceivingPackage(m_el);
    }

    if (!m_el->getDirectedRelationships().count(el.getID())) {
        m_el->getDirectedRelationships().add(el);
    }

    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
}

void Package::RemovePackageMergeFunctor::operator()(PackageMerge& el) const {

    if (el.getReceivingPackage() == m_el) {
        el.setReceivingPackage(0);
    }

    if (m_el->getDirectedRelationships().count(el.getID())) {
        m_el->getDirectedRelationships().remove(el);
    }

    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }
}

void Package::AddProfileApplicationFunctor::operator()(ProfileApplication& el) const {
    if (el.getApplyingPackage() != m_el) {
        el.setApplyingPackage(m_el);
    }

    if (!m_el->getDirectedRelationships().count(el.getID())) {
        m_el->getDirectedRelationships().add(el);
    }

    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
}

void Package::RemoveProfileApplicationFunctor::operator()(ProfileApplication& el) const {
    if (el.getApplyingPackage() == m_el) {
        el.setApplyingPackage(0);
    }

    if (m_el->getDirectedRelationships().count(el.getID())) {
        m_el->getDirectedRelationships().remove(el);
    }

    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }
}

void Package::AddOwnedStereotypeFunctor::operator()(Stereotype& el) const {
    if (!m_el->getPackagedElements().count(el.getID())) {
        m_el->getPackagedElements().add(el);
    }
}

void Package::RemoveOwnedStereotypeFunctor::operator()(Stereotype& el) const {
    if (m_el->getPackagedElements().count(el.getID())) {
        m_el->getPackagedElements().remove(el);
    }
}

void Package::setManager(UmlManager* manager) {
    Namespace::setManager(manager);
    m_packagedElements.m_manager = manager;
    m_packageMerge.m_manager = manager;
    m_profileApplications.m_manager = manager;
    m_ownedStereotypes.m_manager = manager;
}

Package::Package() {
    m_packagedElements.addProcedures.push_back(new AddPackagedElementFunctor(this));
    m_packagedElements.removeProcedures.push_back(new RemovePackagedElementFunctor(this));
    m_packageMerge.addProcedures.push_back(new AddPackageMergeFunctor(this));
    m_packageMerge.removeProcedures.push_back(new RemovePackageMergeFunctor(this));
    m_profileApplications.addProcedures.push_back(new AddProfileApplicationFunctor(this));
    m_profileApplications.removeProcedures.push_back(new RemoveProfileApplicationFunctor(this));
    m_ownedStereotypes.addProcedures.push_back(new AddOwnedStereotypeFunctor(this));
    m_ownedStereotypes.removeProcedures.push_back(new RemoveOwnedStereotypeFunctor(this));
}

Package::~Package() {
    
}

Package::Package(const Package& pckg) : Namespace(pckg), PackageableElement(pckg) , NamedElement(pckg), Element(pckg) {
    m_packagedElements = pckg.m_packagedElements;
    m_packagedElements.m_el = this;
    m_packagedElements.addProcedures.clear();
    m_packagedElements.addProcedures.push_back(new AddPackagedElementFunctor(this));
    m_packagedElements.removeProcedures.clear();
    m_packagedElements.removeProcedures.push_back(new RemovePackagedElementFunctor(this));
    m_packageMerge = pckg.m_packageMerge;
    m_packageMerge.addProcedures.clear();
    m_packageMerge.removeProcedures.clear();
    m_packageMerge.addProcedures.push_back(new AddPackageMergeFunctor(this));
    m_packageMerge.removeProcedures.push_back(new RemovePackageMergeFunctor(this));
    m_profileApplications.addProcedures.clear();
    m_profileApplications.removeProcedures.clear();
    m_profileApplications.addProcedures.push_back(new AddProfileApplicationFunctor(this));
    m_profileApplications.removeProcedures.push_back(new RemoveProfileApplicationFunctor(this));
    m_ownedStereotypes.addProcedures.clear();
    m_ownedStereotypes.removeProcedures.clear();
    m_ownedStereotypes.addProcedures.push_back(new AddOwnedStereotypeFunctor(this));
    m_ownedStereotypes.removeProcedures.push_back(new RemoveOwnedStereotypeFunctor(this));
}

Sequence<PackageableElement>& Package::getPackagedElements() {
    return m_packagedElements;
}

Sequence<PackageMerge>& Package::getPackageMerge() {
    return m_packageMerge;
}

Sequence<ProfileApplication>& Package::getProfileApplications() {
    return m_profileApplications;
}

Sequence<Stereotype>& Package::getOwnedStereotypes() {
    return m_ownedStereotypes;
}

ElementType Package::getElementType() const {
    return ElementType::PACKAGE;
}

bool Package::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = Namespace::isSubClassOf(eType);
    }

    if (!ret) {
        ret = TemplateableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PACKAGE;
    }

    return ret;
}

void Package::restoreReleased(ID id, Element* released) {
    Namespace::restoreReleased(id, released);
    if (m_packagedElements.count(id)) {
        released->as<PackageableElement>().setOwningPackage(this);
    }
}

void Package::referencingReleased(ID id) {
    Namespace::referencingReleased(id);
    m_packagedElements.elementReleased(id);
}