#ifndef PACKAGE_H
#define PACKAGE_H

#include "packageableElement.h"
#include "namespace.h"
#include "templateableElement.h"

namespace UML {

    class Stereotype;
    class ProfileApplication;
    class PackageMerge;

    class Package : public PackageableElement, public Namespace, public TemplateableElement {

        friend class UmlManager;

        protected:
            Set<PackageableElement, Package> m_packagedElements = Set<PackageableElement, Package>(this);
            Set<PackageMerge, Package> m_packageMerge = Set<PackageMerge, Package>(this);
            Set<Stereotype, Package> m_ownedStereotypes = Set<Stereotype, Package>(this);
            Set<ProfileApplication, Package> m_profileApplications = Set<ProfileApplication, Package>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            void init();
            void copy(const Package& rhs);
            Package();
        public:
            virtual ~Package();
            Package(const Package& rhs);
            Set<PackageableElement, Package>& getPackagedElements();
            Set<PackageMerge, Package>& getPackageMerge();
            Set<ProfileApplication, Package>& getProfileApplications();
            Set<Stereotype, Package>& getOwnedStereotypes();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PACKAGE;
            };
    };
}

#endif