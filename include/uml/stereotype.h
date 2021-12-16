#ifndef STEREOTYPE_H
#define STEREOTYPE_H

#include "class.h"

namespace UML {

    class Profile;
    namespace Parsers {
        class SetProfile;
    }

    class Stereotype : public Class {

        friend class UmlManager;
        friend class Parsers::SetProfile;
        friend class Profile;

        private:
            Singleton<Profile, Stereotype> m_profile = Singleton<Profile, Stereotype>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<Profile, Stereotype>& getProfileSingleton();
            void init();
            void copy(const Stereotype& rhs);
            Stereotype();
        public:
            Stereotype(const Stereotype& rhs);
            virtual ~Stereotype();
            Profile* getProfile();
            Profile& getProfileRef();
            ID getProfileID() const;
            bool hasProfile() const;
            void setProfile(Profile& profile);
            void setProfile(Profile* profile);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::STEREOTYPE;
            };
    };
}

#endif