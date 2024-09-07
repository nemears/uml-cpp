#pragma once

#include "packageableElement.h"
#include "namespace.h"
#include "templateableElement.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/indexableSet.h"
#include "uml/types/element.h"
#include <unordered_set>

namespace UML {


    class Package : public PackageableElement, public Namespace, public TemplateableElement {

        friend class PackageImport;
        
        protected:
            class PackageableElementPolicy : public IndexablePolicy {
                    friend class PackageImport;
                protected:
                    std::unordered_set<UmlPtr<PackageImport>> packageImportsAdd;
                    std::unordered_set<UmlPtr<PackageImport>> packageImportsRemove;
                    void elementAdded(PackageableElement& el, Package& me);
                    void elementRemoved(PackageableElement& el, Package& me);
            };
            IndexableSet<PackageableElement, Package, PackageableElementPolicy> m_packagedElements = IndexableSet<PackageableElement, Package, PackageableElementPolicy>(this);
            Set<PackageMerge, Package> m_packageMerge = Set<PackageMerge, Package>(this);
            ReadOnlyIndexableSet<Stereotype, Package> m_ownedStereotypes = ReadOnlyIndexableSet<Stereotype, Package>(this);
            Set<ProfileApplication, Package> m_profileApplications = Set<ProfileApplication, Package>(this);
        public:
            Package(std::size_t elementType, AbstractManager& manager);
            IndexableSet<PackageableElement, Package, PackageableElementPolicy>& getPackagedElements();
            Set<PackageMerge, Package>& getPackageMerge();
            Set<ProfileApplication, Package>& getProfileApplications();
            ReadOnlyIndexableSet<Stereotype, Package>& getOwnedStereotypes();
            typedef TypeInfo<std::tuple<PackageableElement, Namespace/*, TemplateableElement*/>, Package> Info;
    };

    template <>
    struct ElementInfo<Package> {
        static const bool abstract = false;
        inline static const std::string name {"Package"};
        static SetList sets(Package& el) {
            return SetList {
                std::make_pair<std::string, AbstractSet*>("packagedElements", &el.getPackagedElements()),
                std::make_pair<std::string, AbstractSet*>("packageMerges", &el.getPackageMerge()),
                std::make_pair<std::string, AbstractSet*>("profileApplication", &el.getProfileApplications()),
                makeSetPair("ownedStereotypes", el.getOwnedStereotypes())
            };
        }
    };
}
