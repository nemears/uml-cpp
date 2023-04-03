#ifndef _UML_PACKAGE_H_
#define _UML_PACKAGE_H_

#include "packageableElement.h"
#include "namespace.h"
#include "packageMerge.h"
#include "templateableElement.h"
#include "profileApplication.h"

namespace UML {

    class Stereotype;
    class ProfileApplication;
    class PackageMerge;
    void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data);

    class Package : public PackageableElement, public Namespace, public TemplateableElement {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class PackageImport;
        friend void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data);

        protected:
            class AddPackageableElementPolicy {
                friend class PackageImport;
                protected:
                    std::unordered_set<UmlPtr<PackageImport>> packageImportsAdd;
                    void apply(PackageableElement& el, Package& me);
            };
            class RemovePackageableElementPolicy {
                friend class PackageImport;
                protected:
                    std::unordered_set<UmlPtr<PackageImport>> packageImportsRemove;
                    void apply(PackageableElement& el, Package& me);
            };
            CustomSet<PackageableElement, Package, AddPackageableElementPolicy, RemovePackageableElementPolicy> m_packagedElements = CustomSet<PackageableElement, Package, AddPackageableElementPolicy, RemovePackageableElementPolicy>(this);
            CustomSet<PackageMerge, Package> m_packageMerge = CustomSet<PackageMerge, Package>(this);
            CustomReadOnlySet<Stereotype, Package> m_ownedStereotypes = CustomReadOnlySet<Stereotype, Package>(this);
            CustomSet<ProfileApplication, Package> m_profileApplications = CustomSet<ProfileApplication, Package>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
            Package();
        public:
            virtual ~Package();
            Set<PackageableElement, Package>& getPackagedElements();
            Set<PackageMerge, Package>& getPackageMerge();
            Set<ProfileApplication, Package>& getProfileApplications();
            ReadOnlySet<Stereotype, Package>& getOwnedStereotypes();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PACKAGE;
            };
    };
}

#endif