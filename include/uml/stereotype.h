#ifndef _UML_STEREOTYPE_H_
#define _UML_STEREOTYPE_H_

#include "class.h"
#include "profile.h"

namespace UML {

    class Profile;
    typedef UmlPtr<Profile> ProfilePtr;

    class Stereotype : public Class {

        friend class UmlManager;
        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        friend class Profile;

        private:
            Singleton<Profile, Stereotype> m_profile = Singleton<Profile, Stereotype>(this);
            Set<Profile, Stereotype>& getProfileSingleton();
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