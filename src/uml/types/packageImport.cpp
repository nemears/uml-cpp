#include "uml/types/packageImport.h"
#include "uml/types/namespace.h"
#include "uml/types/package.h"
#include "uml/types/packageableElement.h"
#include "uml/uml-stable.h"

using namespace UML;

void PackageImport::ImportedPackagePolicy::elementAdded(Package& el, PackageImport& me) {
    el.m_packagedElements.packageImportsAdd.insert(UmlPtr<PackageImport>(&me));
    el.m_packagedElements.packageImportsRemove.insert(UmlPtr<PackageImport>(&me));

    if (me.getImportingNamespace()) {
        for (auto& pckgedEl : el.getPackagedElements()) {
            if (!me.getImportingNamespace()->getImportedMembers().contains(pckgedEl)) {
                me.getImportingNamespace()->m_importedMembers.add(pckgedEl);
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
                me.getImportingNamespace()->m_importedMembers.remove(pckgedEl.getID());
            }
        }
    }
}

PackageImport::PackageImport() : Element(ElementType::PACKAGE_IMPORT) {
    m_importedPackage.subsets(m_targets);
    m_importingNamespace.subsets(*m_owner);
    m_importingNamespace.subsets(m_sources);
    m_importingNamespace.opposite(&Namespace::getPackageImports);
}

PackageImport::~PackageImport() {

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

bool PackageImport::is(ElementType eType) const {
    bool ret = DirectedRelationship::is(eType);

    if (!ret) {
        ret = eType == ElementType::PACKAGE_IMPORT;
    }

    return ret;
}
