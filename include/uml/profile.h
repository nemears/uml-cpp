#ifndef PROFILE_H
#define PROFILE_H

#include "package.h"

namespace UML {
    class Profile : public Package {

        friend class Stereotype;
        friend class UmlManager;

        protected:
            Set<Stereotype, Profile> m_profileOwnedStereotypes = Set<Stereotype, Profile>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void init();
            void copy(const Profile& rhs);
            Profile();
        public:
            Profile(const Profile& rhs);
            virtual ~Profile();
            Set<Stereotype, Profile>& getOwnedStereotypes();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PROFILE;
            };
    };
}

#endif