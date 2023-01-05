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

using namespace UML;

void PackageImport::AddImportedPackagePolicy::apply(Package& el, PackageImport& me) {
    el.m_packagedElements.packageImportsAdd.insert(UmlPtr<PackageImport>(&me));
    el.m_packagedElements.packageImportsRemove.insert(UmlPtr<PackageImport>(&me));

    if (me.getImportingNamespace()) {
        for (auto& pckgedEl : el.getPackagedElements()) {
            if (!me.getImportingNamespace()->getImportedMembers().contains(pckgedEl)) {
                me.getImportingNamespace()->getImportedMembers().addReadOnly(pckgedEl);
            }
        }
    }
}

void PackageImport::RemoveImportedPackagePolicy::apply(Package& el, PackageImport& me) {
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
                me.getImportingNamespace()->getImportedMembers().removeReadOnly(pckgedEl.getID());
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

IMPLEMENT_SINGLETON(ImportedPackage, m_importedPackage, Package, PackageImport);
IMPLEMENT_SINGLETON(ImportingNamespace, m_importingNamespace, Namespace, PackageImport);

bool PackageImport::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PACKAGE_IMPORT;
    }

    return ret;
}