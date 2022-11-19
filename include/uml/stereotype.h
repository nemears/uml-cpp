#ifndef _UML_STEREOTYPE_H_
#define _UML_STEREOTYPE_H_

#include "class.h"
#include "profile.h"

namespace UML {

    class Profile;
    typedef UmlPtr<Profile> ProfilePtr;

    class Stereotype : public Class {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        friend class Profile;

        private:
            CustomSingleton<Profile, Stereotype> m_profile = CustomSingleton<Profile, Stereotype>(this);
            TypedSet<Profile, Stereotype>& getProfileSingleton();
            void init();
            Stereotype();
        public:
            virtual ~Stereotype();
            ProfilePtr getProfile() const;
            void setProfile(Profile& profile);
            void setProfile(Profile* profile);
            void setProfile(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::STEREOTYPE;
            };
    };
}

#endif