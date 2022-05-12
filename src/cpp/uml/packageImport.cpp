#include "uml/packageImport.h"
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

void PackageImport::AddImportedPackageFunctor::operator()(Element& el) const {
    // add special functor(s) TODO
    PackageAddPackageableElementFunctor* addFunctor = new PackageAddPackageableElementFunctor(&el);
    addFunctor->m_import = &m_el.as<PackageImport>();
    el.as<Package>().getPackagedElements().m_addFunctors.insert(addFunctor);
    PackageRemovePackageableElementFunctor* removeFunctor = new PackageRemovePackageableElementFunctor(&el);
    removeFunctor->m_import = &m_el.as<PackageImport>();
    el.as<Package>().getPackagedElements().m_removeFunctors.insert(removeFunctor);

    if (m_el.as<PackageImport>().getImportingNamespace()) {
        for (auto& pckgedEl : el.as<Package>().getPackagedElements()) {
            if (!m_el.as<PackageImport>().getImportingNamespace()->getImportedMembers().contains(pckgedEl)) {
                m_el.as<PackageImport>().getImportingNamespace()->getImportedMembers().addReadOnly(pckgedEl);
            }
        }
    }
}

void PackageImport::RemoveImportedPackageFunctor::operator()(Element& el) const {
    // remove functor(s)
    for (auto& functor : el.as<Package>().getPackagedElements().m_addFunctors) {
        if (sizeof(functor) == sizeof(PackageAddPackageableElementFunctor)) { // compare type
            el.as<Package>().getPackagedElements().m_addFunctors.erase(functor);
            delete functor;
            break;
        }
    }
    for (auto& functor : el.as<Package>().getPackagedElements().m_removeFunctors) {
        if (sizeof(functor) == sizeof(PackageRemovePackageableElementFunctor)) {
            el.as<Package>().getPackagedElements().m_removeFunctors.erase(functor);
            delete functor;
            break;
        }
    }

    if (m_el.as<PackageImport>().getImportingNamespace()) {
        for (auto& pckgedEl : el.as<Package>().getPackagedElements()) {
            if (m_el.as<PackageImport>().getImportingNamespace()->getImportedMembers().contains(pckgedEl)) {
                m_el.as<PackageImport>().getImportingNamespace()->getImportedMembers().removeReadOnly(pckgedEl.getID());
            }
        }
    }
}

void PackageImport::PackageAddPackageableElementFunctor::operator()(Element& el) const {
    if (m_import->getImportingNamespace()) {
        if (!m_import->getImportingNamespace()->getImportedMembers().contains(el.getID())) {
            m_import->getImportingNamespace()->getImportedMembers().addReadOnly(el.as<PackageableElement>());
        }
    }
}

void PackageImport::PackageRemovePackageableElementFunctor::operator()(Element& el) const {
    if (m_import->getImportingNamespace()) {
        if (m_import->getImportingNamespace()->getImportedMembers().contains(el.getID())) {
            m_import->getImportingNamespace()->getImportedMembers().removeReadOnly(el.getID());
        }
    }
}

Set<Package, PackageImport>& PackageImport::getImportedPackageSingleton() {
    return m_importedPackage;
}

Set<Namespace, PackageImport>& PackageImport::getImportingNamespaceSingleton() {
    return m_importingNamespace;
}

void PackageImport::init() {
    m_importedPackage.subsets(m_targets);
    m_importedPackage.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_importedPackage.m_addFunctors.insert(new AddImportedPackageFunctor(this));
    m_importedPackage.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_importedPackage.m_removeFunctors.insert(new RemoveImportedPackageFunctor(this));
    m_importingNamespace.subsets(*m_owner);
    m_importingNamespace.subsets(m_sources);
    m_importingNamespace.opposite(&Namespace::getPackageImports);
}

PackageImport::PackageImport() : Element(ElementType::PACKAGE_IMPORT) {
    init();
}

PackageImport::~PackageImport() {

}

PackagePtr PackageImport::getImportedPackage() const {
    return m_importedPackage.get();
}

void PackageImport::setImportedPackage(Package* pckg) {
    m_importedPackage.set(pckg);
}

void PackageImport::setImportedPackage(Package& pckg) {
    m_importedPackage.set(pckg);
}

void PackageImport::setImportedPackage(PackagePtr pckg) {
    m_importedPackage.set(pckg);
}

void PackageImport::setImportedPackage(ID id) {
    m_importedPackage.set(id);
}

NamespacePtr PackageImport::getImportingNamespace() const {
    return m_importingNamespace.get();
}

void PackageImport::setImportingNamespace(Namespace* nmspc) {
    m_importingNamespace.set(nmspc);
}

void PackageImport::setImportingNamespace(Namespace& nmspc) {
    m_importingNamespace.set(nmspc);
}

void PackageImport::setImportingNamespace(NamespacePtr nmspc) {
    m_importingNamespace.set(nmspc);
}

void PackageImport::setImportingNamespace(ID id) {
    m_importingNamespace.set(id);
}

bool PackageImport::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PACKAGE_IMPORT;
    }

    return ret;
}