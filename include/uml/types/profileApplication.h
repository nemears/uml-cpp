#pragma once

#include "directedRelationship.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/singleton.h"
#include "uml/types/element.h"

namespace UML {

    class Package;
    class Profile;
    typedef UmlPtr<Profile> ProfilePtr;
    typedef UmlPtr<Package> PackagePtr;

    class ProfileApplication : public DirectedRelationship {

        friend class Package;
        friend struct ElementInfo<ProfileApplication>;

        private:
            Singleton<Profile, ProfileApplication> m_appliedProfile = Singleton<Profile, ProfileApplication>(this);
            Singleton<Package, ProfileApplication> m_applyingPackage = Singleton<Package, ProfileApplication>(this);
            Singleton<Profile, ProfileApplication>& getAppliedProfileSingleton();
            Singleton<Package, ProfileApplication>& getApplyingPackageSingleton();
        public:
            ProfileApplication(std::size_t elementType, AbstractManager& manager);
            ProfilePtr getAppliedProfile() const;
            void setAppliedProfile(Profile& profile);
            void setAppliedProfile(ProfilePtr profile);
            void setAppliedProfile(ID id);
            PackagePtr getApplyingPackage() const;
            void setApplyingPackage(PackagePtr pckg);
            void setApplyingPackage(Package& pckg);
            void setApplyingPackage(ID id);
            typedef TypeInfo<std::tuple<DirectedRelationship>, ProfileApplication> Info;
    };

    template <>
    struct ElementInfo<ProfileApplication> : public DefaultInfo {
        static const bool abstract = false;
        inline static const std::string name {"ProfileApplication"};
        static SetList sets(ProfileApplication& el) {
            return SetList{
                makeSetPair("appliedProfile", el.m_appliedProfile),
                makeSetPair("applyingPackage", el.m_applyingPackage)
            };
        }
    };
}
