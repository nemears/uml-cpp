#ifndef PROFILE_H
#define PROFILE_H

#include "package.h"

namespace UML {
    class Profile : public Package {

        friend class Stereotype;

        private:
        public:
            Profile();
            Profile(const Profile& profile);
            virtual ~Profile();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PROFILE;
            };
    };
}

#endif