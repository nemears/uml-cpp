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

    class Package : public PackageableElement, public Namespace, public TemplateableElement {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSet<PackageableElement, Package> m_packagedElements = CustomSet<PackageableElement, Package>(this);
            CustomSet<PackageMerge, Package> m_packageMerge = CustomSet<PackageMerge, Package>(this);
            CustomSet<Stereotype, Package> m_ownedStereotypes = CustomSet<Stereotype, Package>(this);
            CustomSet<ProfileApplication, Package> m_profileApplications = CustomSet<ProfileApplication, Package>(this);
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void init();
            Package();
        public:
            virtual ~Package();
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