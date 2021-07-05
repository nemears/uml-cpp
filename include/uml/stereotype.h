#ifndef STEREOTYPE_H
#define STEREOTYPE_H

#include "class.h"

namespace UML {

    class Profile;

    class Stereotype : public Class {
        private:
            ID m_profileID;
            Profile* m_profilePtr;
        public:
            Stereotype();
            Stereotype(const Stereotype& stereotype);
            virtual ~Stereotype();
            Profile* getProfile();
            // move to protected
            void setProfile(Profile* profile);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::STEREOTYPE;
            };
    };
}

#endif