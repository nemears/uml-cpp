#pragma once

#include "directedRelationship.h"
#include "uml/set/singleton.h"

namespace UML {

    class Package;
    class Profile;
    typedef UmlPtr<Profile> ProfilePtr;
    typedef UmlPtr<Package> PackagePtr;

    class ProfileApplication : public DirectedRelationship {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Package;

        private:
            Singleton<Profile, ProfileApplication> m_appliedProfile = Singleton<Profile, ProfileApplication>(this);
            Singleton<Package, ProfileApplication> m_applyingPackage = Singleton<Package, ProfileApplication>(this);
            Singleton<Profile, ProfileApplication>& getAppliedProfileSingleton();
            Singleton<Package, ProfileApplication>& getApplyingPackageSingleton();
            ProfileApplication();
        public:
            virtual ~ProfileApplication();
            ProfilePtr getAppliedProfile() const;
            void setAppliedProfile(Profile& profile);
            void setAppliedProfile(ProfilePtr profile);
            void setAppliedProfile(ID id);
            PackagePtr getApplyingPackage() const;
            void setApplyingPackage(PackagePtr pckg);
            void setApplyingPackage(Package& pckg);
            void setApplyingPackage(ID id);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PROFILE_APPLICATION;
            };
    };
}
