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

Package::Package() {
    m_packagedElements = new Sequence<PackageableElement>();
    m_packagedElements->addProcedures.push_back(new AddPackagedElementFunctor(this));
    m_packagedElements->removeProcedures.push_back(new RemovePackagedElementFunctor(this));
}

Package::~Package() {
    delete m_packagedElements;
}

Package::Package(const Package& pckg) : Namespace(pckg), PackageableElement(pckg), NamedElement(pckg), Element(pckg) {
    m_packagedElements = new Sequence<PackageableElement>(*pckg.m_packagedElements);
    m_packagedElements->addProcedures.clear();
    m_packagedElements->addProcedures.push_back(new AddPackagedElementFunctor(this));
    m_packagedElements->removeProcedures.clear();
    m_packagedElements->removeProcedures.push_back(new RemovePackagedElementFunctor(this));
}

Sequence<PackageableElement>& Package::getPackagedElements() {
    return *m_packagedElements;
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