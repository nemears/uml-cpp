#pragma once

#include "packageableElement.h"
#include "namespace.h"
// #include "packageMerge.h"
// #include "templateableElement.h"
// #include "profileApplication.h"

namespace UML {

    class Stereotype;
    class ProfileApplication;
    class PackageMerge;
    void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data);

    class Package : public PackageableElement, public Namespace/*, public TemplateableElement*/ {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class PackageImport;
        friend void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data);

        protected:
            class PackageableElementPolicy {
                friend class PackageImport;
                protected:
                    // std::unordered_set<UmlPtr<PackageImport>> packageImportsAdd;
                    void elementAdded(PackageableElement& el, Package& me);
                    void elementRemoved(PackageableElement& el, Package& me);
            };
            Set<PackageableElement, Package, PackageableElementPolicy> m_packagedElements = Set<PackageableElement, Package, PackageableElementPolicy>(this);
            // Set<PackageMerge, Package> m_packageMerge = CustomSet<PackageMerge, Package>(this);
            // ReadOnlySet<Stereotype, Package> m_ownedStereotypes = CustomReadOnlySet<Stereotype, Package>(this);
            // Set<ProfileApplication, Package> m_profileApplications = CustomSet<ProfileApplication, Package>(this);
            void referenceErased(ID id) override;
            Package();
        public:
            virtual ~Package();
            Set<PackageableElement, Package, PackageableElementPolicy>& getPackagedElements();
            // Set<PackageMerge, Package>& getPackageMerge();
            // Set<ProfileApplication, Package>& getProfileApplications();
            // ReadOnlySet<Stereotype, Package>& getOwnedStereotypes();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PACKAGE;
            };
    };
}
