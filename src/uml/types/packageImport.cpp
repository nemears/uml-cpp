#include "uml/managers/abstractManager.h"
#include "uml/types/directedRelationship.h"
#include "uml/uml-stable.h"

using namespace UML;

void PackageImport::ImportedPackagePolicy::elementAdded(Package& el, PackageImport& me) {
    el.m_packagedElements.packageImportsAdd.insert(UmlPtr<PackageImport>(&me));
    el.m_packagedElements.packageImportsRemove.insert(UmlPtr<PackageImport>(&me));

    if (me.getImportingNamespace()) {
        for (auto& pckgedEl : el.getPackagedElements()) {
            if (!me.getImportingNamespace()->getImportedMembers().contains(pckgedEl)) {
                me.addToReadonlySet(me.getImportingNamespace()->m_importedMembers, pckgedEl);
            }
        }
    }
}

void PackageImport::ImportedPackagePolicy::elementRemoved(Package& el, PackageImport& me) {
    std::unordered_set<UmlPtr<PackageImport>>::iterator addIt = el.m_packagedElements.packageImportsAdd.find(UmlPtr<PackageImport>(&me));
    if (addIt != el.m_packagedElements.packageImportsAdd.end()) {
        el.m_packagedElements.packageImportsAdd.erase(addIt);
    }
    std::unordered_set<UmlPtr<PackageImport>>::iterator removeIt = el.m_packagedElements.packageImportsRemove.find(UmlPtr<PackageImport>(&me));
    if (removeIt != el.m_packagedElements.packageImportsRemove.end()) {
        el.m_packagedElements.packageImportsRemove.erase(removeIt);
    }

    if (me.getImportingNamespace()) {
        for (auto& pckgedEl : el.getPackagedElements()) {
            if (me.getImportingNamespace()->getImportedMembers().contains(pckgedEl)) {
                me.removeFromReadonlySet(me.getImportingNamespace()->m_importedMembers, pckgedEl);
            }
        }
    }
}

Singleton<Namespace, PackageImport>& PackageImport::getImportingNamespaceSingleton() {
    return m_importingNamespace;
}

PackageImport::PackageImport(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    DirectedRelationship(elementType, manager)
{
    m_importedPackage.subsets(m_targets);
    m_importingNamespace.subsets(m_owner);
    m_importingNamespace.subsets(m_sources);
    m_importingNamespace.opposite(&Namespace::getPackageImports);
}

void PackageImport::setImportedPackage(ID id) {
    m_importedPackage.set(id);
}

void PackageImport::setImportedPackage(Package& pckg) {
    m_importedPackage.set(pckg);
}

void PackageImport::setImportedPackage(PackagePtr pckg) {
    m_importedPackage.set(pckg);
}

PackagePtr PackageImport::getImportedPackage() const {
    return m_importedPackage.get();
}

void PackageImport::setImportingNamespace(ID id) {
    m_importingNamespace.set(id);
}

void PackageImport::setImportingNamespace(Namespace& nmspc) {
    m_importingNamespace.set(nmspc);
}

void PackageImport::setImportingNamespace(NamespacePtr nmspc) {
    m_importingNamespace.set(nmspc);
}

NamespacePtr PackageImport::getImportingNamespace() const {
    return m_importingNamespace.get();
}

VisibilityKind PackageImport::getVisibility() const {
    return m_visibility;
}
void PackageImport::setVisibility(VisibilityKind visibility) {
    m_visibility = visibility;
}
