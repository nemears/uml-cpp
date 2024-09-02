#pragma once

#include "packageableElement.h"
#include "namespace.h"
// #include "templateableElement.h"
#include "uml/set/indexableSet.h"
#include <unordered_set>

namespace UML {


    class Package : public PackageableElement, public Namespace/*, public TemplateableElement*/ {

        friend class PackageImport;
        
        template <class T>        
        friend class Creator;

        protected:
            class PackageableElementPolicy : public IndexablePolicy {
                    friend class PackageImport;
                protected:
                    std::unordered_set<UmlPtr<PackageImport>> packageImportsAdd;
                    std::unordered_set<UmlPtr<PackageImport>> packageImportsRemove;
                    void elementAdded(PackageableElement& el, Package& me);
                    void elementRemoved(PackageableElement& el, Package& me);
            };
            typedef TypeInfo<std::tuple<PackageableElement, Namespace, TemplateableElement>> Info;
            IndexableSet<PackageableElement, Package, PackageableElementPolicy> m_packagedElements = IndexableSet<PackageableElement, Package, PackageableElementPolicy>(this);
            // Set<PackageMerge, Package> m_packageMerge = Set<PackageMerge, Package>(this);
            // ReadOnlyIndexableSet<Stereotype, Package> m_ownedStereotypes = ReadOnlyIndexableSet<Stereotype, Package>(this);
            // Set<ProfileApplication, Package> m_profileApplications = Set<ProfileApplication, Package>(this);
            Package(std::size_t elementType, AbstractManager& manager);
        public:
            IndexableSet<PackageableElement, Package, PackageableElementPolicy>& getPackagedElements();
            // Set<PackageMerge, Package>& getPackageMerge();
            // Set<ProfileApplication, Package>& getProfileApplications();
            // ReadOnlyIndexableSet<Stereotype, Package>& getOwnedStereotypes();
    };
}
