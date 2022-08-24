#ifndef _UML_PROFILE_H_
#define _UML_PROFILE_H_

#include "package.h"

namespace UML {
    class Profile : public Package {

        friend class Stereotype;
        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Set<Stereotype, Profile> m_profileOwnedStereotypes = Set<Stereotype, Profile>(this);
            void init();
            Profile();
        public:
            virtual ~Profile();
            Set<Stereotype, Profile>& getOwnedStereotypes();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PROFILE;
            };
    };
}

#endif