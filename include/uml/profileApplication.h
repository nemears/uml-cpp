#ifndef _UML_PROFILE_APPLICATION_H_
#define _UML_PROFILE_APPLICATION_H_

#include "uml/directedRelationship.h"
#include "uml/singleton.h"

namespace UML {

    class Profile;
    typedef UmlPtr<Profile> ProfilePtr;
    typedef UmlPtr<Package> PackagePtr;

    class ProfileApplication : public DirectedRelationship {

        friend class UmlManager;
        friend class Package;

        private:
            Singleton<Profile, ProfileApplication> m_appliedProfile = Singleton<Profile, ProfileApplication>(this);
            Singleton<Package, ProfileApplication> m_applyingPackage = Singleton<Package, ProfileApplication>(this);
            Set<Profile, ProfileApplication>& getAppliedProfileSingleton();
            Set<Package, ProfileApplication>& getApplyingPackageSingleton();
            void init();
            ProfileApplication();
        public:
            virtual ~ProfileApplication();
            ProfilePtr getAppliedProfile() const;
            void setAppliedProfile(Profile& profile);
            void setAppliedProfile(Profile* profile);
            void setAppliedProfile(ID id);
            PackagePtr getApplyingPackage();
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