#ifndef _UML_PROFILE_APPLICATION_H_
#define _UML_PROFILE_APPLICATION_H_

#include "directedRelationship.h"
#include "set/singleton.h"

namespace UML {

    class Package;
    class Profile;
    typedef UmlPtr<Profile> ProfilePtr;
    typedef UmlPtr<Package> PackagePtr;

    class ProfileApplication : public DirectedRelationship {

        template <typename AccessPolicy, typename SerializationPolicy> friend class Manager;
        friend class Package;

        private:
            CustomSingleton<Profile, ProfileApplication> m_appliedProfile = CustomSingleton<Profile, ProfileApplication>(this);
            CustomSingleton<Package, ProfileApplication> m_applyingPackage = CustomSingleton<Package, ProfileApplication>(this);
            TypedSet<Profile, ProfileApplication>& getAppliedProfileSingleton();
            TypedSet<Package, ProfileApplication>& getApplyingPackageSingleton();
            ProfileApplication();
        public:
            virtual ~ProfileApplication();
            ProfilePtr getAppliedProfile() const;
            void setAppliedProfile(Profile& profile);
            void setAppliedProfile(Profile* profile);
            void setAppliedProfile(ID id);
            PackagePtr getApplyingPackage() const;
            void setApplyingPackage(Package* pckg);
            void setApplyingPackage(Package& pckg);
            void setApplyingPackage(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PROFILE_APPLICATION;
            };
    };
}

#endif