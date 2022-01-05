#ifndef PROFILE_APPLICATION_H
#define PROFILE_APPLICATION_H

#include "uml/directedRelationship.h"
#include "uml/singleton.h"
#include "yaml-cpp/yaml.h"

namespace UML {

    class Profile;

    namespace Parsers {
        void parseProfileApplication(YAML::Node node, ProfileApplication& application, Parsers::ParserMetaData& data);
    }

    class ProfileApplication : public DirectedRelationship {

        friend class UmlManager;
        friend class Package;
        friend void Parsers::parseProfileApplication(YAML::Node node, ProfileApplication& application, Parsers::ParserMetaData& data);

        private:
            Singleton<Profile, ProfileApplication> m_appliedProfile = Singleton<Profile, ProfileApplication>(this);
            Singleton<Package, ProfileApplication> m_applyingPackage = Singleton<Package, ProfileApplication>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<Profile, ProfileApplication>& getAppliedProfileSingleton();
            Set<Package, ProfileApplication>& getApplyingPackageSingleton();
            void init();
            void copy(const ProfileApplication& rhs);
            ProfileApplication();
        public:
            ProfileApplication(const ProfileApplication& rhs);
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
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PROFILE_APPLICATION;
            };
    };
}

#endif