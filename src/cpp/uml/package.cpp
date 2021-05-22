#include "uml/package.h"

using namespace UML;

void Package::AddPackagedElementFunctor::operator()(Element& el) const {
    if (dynamic_cast<PackageableElement&>(el).getOwningPackage() != m_el) {
        dynamic_cast<PackageableElement&>(el).setOwningPackage(dynamic_cast<Package*>(m_el));
    }

    if (!dynamic_cast<Namespace*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<Namespace*>(m_el)->getOwnedMembers().add(dynamic_cast<NamedElement&>(el));
    }
}

void Package::RemovePackagedElementFunctor::operator()(Element& el) const {
    if (dynamic_cast<PackageableElement&>(el).getOwningPackage() == m_el) {
        dynamic_cast<PackageableElement&>(el).setOwningPackage(0);
    }

    if (dynamic_cast<Package*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<Package*>(m_el)->getMembers().remove(dynamic_cast<PackageableElement&>(el));
    }
}

void Package::AddPackageMergeFunctor::operator()(Element& el) const {
    if (dynamic_cast<PackageMerge&>(el).getReceivingPackage() != m_el) {
        dynamic_cast<PackageMerge&>(el).setReceivingPackage(dynamic_cast<Package*>(m_el));
    }

    if (!dynamic_cast<Package*>(m_el)->getDirectedRelationships().count(el.getID())) {
        dynamic_cast<Package*>(m_el)->getDirectedRelationships().add(dynamic_cast<PackageMerge&>(el));
    }

    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
}

void Package::RemovePackageMergeFunctor::operator()(Element& el) const {

    if (dynamic_cast<PackageMerge&>(el).getReceivingPackage() == m_el) {
        dynamic_cast<PackageMerge&>(el).setReceivingPackage(0);
    }

    if (dynamic_cast<Package*>(m_el)->getDirectedRelationships().count(el.getID())) {
        dynamic_cast<Package*>(m_el)->getDirectedRelationships().remove(dynamic_cast<PackageMerge&>(el));
    }

    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().remove(el);
    }
}

Package::Package() {
    m_packagedElements.addProcedures.push_back(new AddPackagedElementFunctor(this));
    m_packagedElements.removeProcedures.push_back(new RemovePackagedElementFunctor(this));
    m_packageMerge.addProcedures.push_back(new AddPackageMergeFunctor(this));
    m_packageMerge.removeProcedures.push_back(new RemovePackageMergeFunctor(this));
}

Package::~Package() {
    
}

Package::Package(const Package& pckg) : Namespace(pckg), PackageableElement(pckg) , NamedElement(pckg), Element(pckg) {
    m_packagedElements = pckg.m_packagedElements;
    m_packagedElements.addProcedures.clear();
    m_packagedElements.addProcedures.push_back(new AddPackagedElementFunctor(this));
    m_packagedElements.removeProcedures.clear();
    m_packagedElements.removeProcedures.push_back(new RemovePackagedElementFunctor(this));
    m_packageMerge = pckg.m_packageMerge;
    m_packageMerge.addProcedures.clear();
    m_packageMerge.removeProcedures.clear();
    m_packageMerge.addProcedures.push_back(new AddPackageMergeFunctor(this));
    m_packageMerge.removeProcedures.push_back(new RemovePackageMergeFunctor(this));
}

Sequence<PackageableElement>& Package::getPackagedElements() {
    return m_packagedElements;
}

Sequence<PackageMerge>& Package::getPackageMerge() {
    return m_packageMerge;
}

ElementType Package::getElementType() const {
    return ElementType::PACKAGE;
}

bool Package::isSubClassOf(ElementType eType) {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = Namespace::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PACKAGE;
    }

    return ret;
}