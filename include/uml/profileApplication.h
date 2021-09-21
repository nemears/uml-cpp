#ifndef PROFILE_APPLICATION_H
#define PROFILE_APPLICATION_H

#include "uml/directedRelationship.h"

namespace UML {

    class Profile;

    class ProfileApplication : public DirectedRelationship {

        friend class UmlManager;

        private:
            Singleton<Profile, ProfileApplication> m_appliedProfile = Singleton<Profile, ProfileApplication>(this);
            class RemoveAppliedProfileProcedure : public AbstractSingletonProcedure<Profile, ProfileApplication> {
                public:
                    RemoveAppliedProfileProcedure(ProfileApplication* me) : AbstractSingletonProcedure<Profile, ProfileApplication>(me) {};
                    void operator()(Profile* el) const override;
            };
            class AddAppliedProfileProcedure : public AbstractSingletonProcedure<Profile, ProfileApplication> {
                public:
                    AddAppliedProfileProcedure(ProfileApplication* me) : AbstractSingletonProcedure<Profile, ProfileApplication>(me) {};
                    void operator()(Profile* el) const override;
            };
            Singleton<Package, ProfileApplication> m_applyingPackage = Singleton<Package, ProfileApplication>(this);
            class RemoveApplyingPackageProcedure : public AbstractSingletonProcedure<Package, ProfileApplication> {
                public:
                    RemoveApplyingPackageProcedure(ProfileApplication* me) : AbstractSingletonProcedure<Package, ProfileApplication>(me) {};
                    void operator()(Package* el) const override;
            };
            class AddApplyingPackageProcedure : public AbstractSingletonProcedure<Package, ProfileApplication> {
                public:
                    AddApplyingPackageProcedure(ProfileApplication* me) : AbstractSingletonProcedure<Package, ProfileApplication>(me) {};
                    void operator()(Package* el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            ProfileApplication();
        public:
            ProfileApplication(const ProfileApplication& profileApplication);
            virtual ~ProfileApplication();
            Profile* getAppliedProfile();
            Profile& getAppliedProfileRef();
            ID getAppliedProfileID() const;
            bool hasAppliedProfile() const;
            void setAppliedProfile(Profile& profile);
            void setAppliedProfile(Profile* profile);
            Package* getApplyingPackage();
            Package& getApplyingPackageRef();
            ID getApplyingPackageID() const;
            bool hasApplyingPackage() const;
            void setApplyingPackage(Package* pckg);
            void setApplyingPackage(Package& pckg);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PROFILE_APPLICATION;
            };
    };
}

#endif