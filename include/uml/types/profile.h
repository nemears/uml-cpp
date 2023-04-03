#ifndef _UML_PROFILE_H_
#define _UML_PROFILE_H_

#include "package.h"

namespace UML {
    class Profile : public Package {

        friend class Stereotype;
        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomReadOnlySet<Stereotype, Profile> m_profileOwnedStereotypes = CustomReadOnlySet<Stereotype, Profile>(this);
            Profile();
        public:
            virtual ~Profile();
            ReadOnlySet<Stereotype, Profile>& getOwnedStereotypes();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PROFILE;
            };
    };
}

#endif