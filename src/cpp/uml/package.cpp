#include "uml/package.h"

using namespace UML;

void Package::AddPackagedElementFunctor::operator()(Element& el) const {
    if (dynamic_cast<PackageableElement&>(el).getOwningPackage() != m_el) {
        dynamic_cast<PackageableElement&>(el).setOwningPackage(dynamic_cast<Package*>(m_el));
    }

    if (!dynamic_cast<Namespace*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<Namespace*>(m_el)->getMembers().add(dynamic_cast<NamedElement&>(el));
    }
}

Package::Package() {
    m_packagedElements = new Sequence<PackageableElement>();
    m_packagedElements->addProcedures.push_back(new AddPackagedElementFunctor(this));
}

Package::~Package() {
    delete m_packagedElements;
}

Sequence<PackageableElement>& Package::getPackagedElements() {
    return *m_packagedElements;
}

ElementType Package::getElementType() {
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