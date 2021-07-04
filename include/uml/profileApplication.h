#ifndef PROFILE_APPLICATION_H
#define PROFILE_APPLICATION_H

#include "uml/directedRelationship.h"

namespace UML {

    class Profile;

    class ProfileApplication : public DirectedRelationship {
        private:
            ID m_appliedProfileID;
            Profile* m_appliedProfilePtr;
            ID m_applyingPackageID;
            Package* m_applyingPackagePtr;
        public:
            ProfileApplication();
            ProfileApplication(const ProfileApplication& profileApplication);
            virtual ~ProfileApplication();
            Profile* getAppliedProfile();
            void setAppliedProfile(Profile* profile);
            Package* getApplyingPackage();
            void setApplyingPackage(Package* pckg);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PROFILE_APPLICATION;
            };
    };
}

#endif