#pragma once 

#include "package.h"

namespace UML {
    class Profile : public Package {

        friend class Stereotype;
        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Profile();
        public:
            virtual ~Profile();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PROFILE;
            };
    };
}
