#ifndef PROFILE_APPLICATION_H
#define PROFILE_APPLICATION_H

#include "uml/directedRelationship.h"

namespace UML {
    class ProfileApplication : public DirectedRelationship {
        private:
        public:
            ProfileApplication();
            ProfileApplication(const ProfileApplication& profileApplication);
            virtual ~ProfileApplication();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PROFILE_APPLICATION;
            };
    };
}

#endif