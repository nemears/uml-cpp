#pragma once

#include "packageableElement.h"
#include "namespace.h"
#include "templateableElement.h"
#include "uml/set/indexableSet.h"
#include <unordered_set>

namespace UML {

    class Stereotype;
    class ProfileApplication;
    class PackageMerge;
    class PackageImport;
    void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data);

    class Package : public PackageableElement, public Namespace, public TemplateableElement {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class PackageImport;
        friend void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data);

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
            void referenceErased(ID id) override;
            Package();
        public:
            virtual ~Package();
            IndexableSet<PackageableElement, Package, PackageableElementPolicy>& getPackagedElements();
            Set<PackageMerge, Package>& getPackageMerge();
            Set<ProfileApplication, Package>& getProfileApplications();
            ReadOnlyIndexableSet<Stereotype, Package>& getOwnedStereotypes();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PACKAGE;
            };
    };
}
