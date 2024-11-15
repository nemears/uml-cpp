#pragma once

#include "packageableElement.h"
#include "uml/managers/templateTypeList.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/indexableSet.h"
#include "uml/types/element.h"

namespace UML {

    template <class ManagerPolicy>
    class Package : public ManagerPolicy {

        template <class>
        friend class PackageImport;
        friend struct ElementInfo<Package<ManagerPolicy>>;
        
        using ManagedPackageImport = PackageImport<typename ManagerPolicy::Manager::template GenBaseHierarchy<PackageImport>>;
        using ManagedPackageableElement = PackageableElement<typename ManagerPolicy::Manager::template GenBaseHierarchy<PackageableElement>>;
        using ManagedPackageMerge = PackageMerge<typename ManagerPolicy::Manager::template GenBaseHierarchy<PackageMerge>>;
        using ManagedProfileApplication = ProfileApplication<typename ManagerPolicy::Manager::template GenBaseHierarchy<ProfileApplication>>;
        
        protected:
            class PackageableElementPolicy : public IndexablePolicy<ManagerPolicy> {
                    template <class>
                    friend class PackageImport;
                protected:
                    std::unordered_set<UmlPtr<ManagedPackageImport>> packageImportsAdd;
                    std::unordered_set<UmlPtr<ManagedPackageImport>> packageImportsRemove;
                    void elementAdded(ManagedPackageableElement& el, Package& me) {
                        // for (const UmlPtr<ManagedPackageImport>& import : packageImportsAdd) {
                        //     if (import->getImportingNamespace()) {
                        //         if (!import->getImportingNamespace()->getImportedMembers().contains(el.getID())) {
                        //             // import->getImportingNamespace()->m_importedMembers.innerAdd(ElementPtr(&el));
                        //             me.addToReadonlySet(import->getImportingNamespace()->m_importedMembers, el);
                        //         }
                        //     }
                        // }
                        // if (el.template is<Stereotype>() && !me.getOwnedStereotypes().contains(el.getID())) {
                        //     me.m_ownedStereotypes.innerAdd(ElementPtr(&el));
                        // } 
                    }
                    void elementRemoved(ManagedPackageableElement& el, Package& me) {
                        // for (const UmlPtr<ManagedPackageImport>& import : packageImportsRemove) {
                        //     if (import->getImportingNamespace()) {
                        //         if (import->getImportingNamespace()->getImportedMembers().contains(el.getID())) {
                        //             me.removeFromReadonlySet(import->getImportingNamespace()->m_importedMembers, el);
                        //         }
                        //     }
                        // }
                        // if (el.template is<Stereotype>() && me.getOwnedStereotypes().contains(el.getID())) {
                        //     me.m_ownedStereotypes.innerRemove(ElementPtr(&el));
                        // }                     
                    }
            };
            IndexableSet<PackageableElement, Package, PackageableElementPolicy> m_packagedElements = IndexableSet<PackageableElement, Package, PackageableElementPolicy>(this);
            // Set<PackageMerge, Package> m_packageMerge = Set<PackageMerge, Package>(this);
            // ReadOnlyIndexableSet<Stereotype, Package> m_ownedStereotypes = ReadOnlyIndexableSet<Stereotype, Package>(this);
            // Set<ProfileApplication, Package> m_profileApplications = Set<ProfileApplication, Package>(this);
        public:
            Package(std::size_t elementType, AbstractManager& manager) : ManagerPolicy(elementType, manager) {
                m_packagedElements.subsets(ManagerPolicy::m_ownedMembers);
                m_packagedElements.opposite(&ManagedPackageableElement::getOwningPackageSingleton);
                // m_packageMerge.subsets(ManagerPolicy::m_ownedElements);
                // m_packageMerge.opposite(&ManagedPackageMerge::getReceivingPackageSingleton);
                // m_ownedStereotypes.subsets(m_packagedElements);
                // m_profileApplications.subsets(ManagerPolicy::m_ownedElements);
                // m_profileApplications.opposite(&ManagedProfileApplication::getApplyingPackageSingleton); 
            }
            IndexableSet<PackageableElement, Package, PackageableElementPolicy>& getPackagedElements() {
                return m_packagedElements;
            }
            // Set<PackageMerge, Package>& getPackageMerge() {
            //     return m_packageMerge;
            // }
            // Set<ProfileApplication, Package>& getProfileApplications() {
            //     return m_profileApplications;
            // }
            // ReadOnlyIndexableSet<Stereotype, Package>& getOwnedStereotypes() {
            //     return m_ownedStereotypes;
            // }
            typedef TypeInfo<TemplateTypeList<PackageableElement, Namespace/**, TemplateableElement**/>, Package> Info;
    };

    template <class ManagerPolicy>
    struct ElementInfo<Package<ManagerPolicy>> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Package"; }
        static SetList sets(Package<ManagerPolicy>& el) {
            return SetList {
                makeSetPair("packagedElements", el.m_packagedElements),
                makeSetPair("packageMerges", el.m_packageMerge),
                makeSetPair("profileApplications", el.m_profileApplications),
                makeSetPair("ownedStereotypes", el.m_ownedStereotypes)
            };
        }
    };
}
