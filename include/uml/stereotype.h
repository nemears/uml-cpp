#ifndef STEREOTYPE_H
#define STEREOTYPE_H

#include "class.h"

namespace UML {

    class Profile;

    class Stereotype : public Class {

        friend class UmlManager;

        private:
            Singleton<Profile, Stereotype> m_profile = Singleton<Profile, Stereotype>(this);
            class RemoveProfileProcedure : public AbstractSingletonProcedure<Profile, Stereotype> {
                public:
                    RemoveProfileProcedure(Stereotype* me) : AbstractSingletonProcedure<Profile, Stereotype>(me) {};
                    void operator()(Profile* el) const override;
            };
            class AddProfileProcedure : public AbstractSingletonProcedure<Profile, Stereotype> {
                public:
                    AddProfileProcedure(Stereotype* me) : AbstractSingletonProcedure<Profile, Stereotype>(me) {};
                    void operator()(Profile* el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            Stereotype();
        public:
            Stereotype(const Stereotype& stereotype);
            virtual ~Stereotype();
            Profile* getProfile();
            Profile& getProfileRef();
            bool hasProfile() const;
            void setProfile(Profile& profile);
            void setProfile(Profile* profile);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::STEREOTYPE;
            };
    };
}

#endif