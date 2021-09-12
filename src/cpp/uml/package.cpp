#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/packageMerge.h"
#include "uml/profileApplication.h"

using namespace UML;

void Package::AddPackagedElementFunctor::operator()(PackageableElement& el) const {
    oppositeSingletonAdd(el, &PackageableElement::setOwningPackage);
    subsetsAdd<Namespace, NamedElement>(el, &Namespace::getOwnedMembers);
    updateCopiedSequenceAddedTo(el, &Package::getPackagedElements);
}

void Package::RemovePackagedElementFunctor::operator()(PackageableElement& el) const {
    oppositeSingletonRemove(el, &PackageableElement::m_owningPackage);
    subsetsRemove<Namespace, NamedElement>(el, &Namespace::getMembers);
    updateCopiedSequenceRemovedFrom(el, &Package::getPackagedElements);
}

void Package::AddPackageMergeFunctor::operator()(PackageMerge& el) const {
    oppositeSingletonAdd(el, &PackageMerge::setReceivingPackage);
    subsetsAdd<Element, DirectedRelationship>(el, &Element::getDirectedRelationships);
    subsetsAdd<Element, Element>(el, &Element::getOwnedElements);
    updateCopiedSequenceAddedTo(el, &Package::getPackageMerge);
}

void Package::RemovePackageMergeFunctor::operator()(PackageMerge& el) const {
    oppositeSingletonRemove(el, &PackageMerge::m_receivingPackage);
    subsetsRemove<Element, DirectedRelationship>(el, &Element::getDirectedRelationships);
    subsetsRemove<Element, Element>(el, &Element::getOwnedElements);
    updateCopiedSequenceRemovedFrom(el, &Package::getPackageMerge);
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
    updateCopiedSequenceAddedTo(el, &Package::getProfileApplications);
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
    updateCopiedSequenceRemovedFrom(el, &Package::getProfileApplications);
}

void Package::AddOwnedStereotypeFunctor::operator()(Stereotype& el) const {
    if (!m_el->getPackagedElements().count(el.getID())) {
        m_el->getPackagedElements().add(el);
    }
    updateCopiedSequenceAddedTo(el, &Package::getOwnedStereotypes);
}

void Package::RemoveOwnedStereotypeFunctor::operator()(Stereotype& el) const {
    if (m_el->getPackagedElements().count(el.getID())) {
        m_el->getPackagedElements().remove(el);
    }
    updateCopiedSequenceRemovedFrom(el, &Package::getOwnedStereotypes);
}

void Package::setManager(UmlManager* manager) {
    Namespace::setManager(manager);
    m_packagedElements.m_manager = manager;
    m_packageMerge.m_manager = manager;
    m_profileApplications.m_manager = manager;
    m_ownedStereotypes.m_manager = manager;
}

void Package::referenceReindexed(ID oldID, ID newID) {
    Namespace::referenceReindexed(oldID, newID);
    PackageableElement::referenceReindexed(oldID, newID);
    if (m_packagedElements.count(oldID)) {
        m_packagedElements.reindex(oldID, newID, &Package::getPackagedElements);
    }
    if (m_packageMerge.count(oldID)) {
        m_packageMerge.reindex(oldID, newID, &Package::getPackageMerge);
    }
    if (m_profileApplications.count(oldID)) {
        m_profileApplications.reindex(oldID, newID);
    }
    if (m_ownedStereotypes.count(oldID)) {
        m_ownedStereotypes.reindex(oldID, newID);
    }
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
    m_packageMerge.m_el = this;
    m_packageMerge.addProcedures.clear();
    m_packageMerge.removeProcedures.clear();
    m_packageMerge.addProcedures.push_back(new AddPackageMergeFunctor(this));
    m_packageMerge.removeProcedures.push_back(new RemovePackageMergeFunctor(this));
    m_profileApplications.m_el = this;
    m_profileApplications.addProcedures.clear();
    m_profileApplications.removeProcedures.clear();
    m_profileApplications.addProcedures.push_back(new AddProfileApplicationFunctor(this));
    m_profileApplications.removeProcedures.push_back(new RemoveProfileApplicationFunctor(this));
    m_ownedStereotypes.m_el = this;
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
    if (m_packageMerge.count(id)) {
        released->as<PackageMerge>().setReceivingPackage(this);
    }
    if (m_profileApplications.count(id)) {
        released->as<ProfileApplication>().setApplyingPackage(this);
    }
}

void Package::referencingReleased(ID id) {
    Namespace::referencingReleased(id);
    m_packagedElements.elementReleased(id, &Package::getPackagedElements);
    m_packageMerge.elementReleased(id, &Package::getPackageMerge);
    m_profileApplications.elementReleased(id, &Package::getProfileApplications);
    m_ownedStereotypes.elementReleased(id, &Package::getOwnedStereotypes);
}