#pragma once

#include "class.h"

namespace UML {

    class Profile;
    typedef UmlPtr<Profile> ProfilePtr;

    class Stereotype : public Class {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Profile;

        private:

            struct OwningPackagePolicy {
                void elementAdded(Package& el, Stereotype& me);
                void elementRemoved(Package& el, Stereotype& me);
            };

            Singleton<Profile, Stereotype> m_profile = Singleton<Profile, Stereotype>(this);
            Singleton<Package, Stereotype, OwningPackagePolicy> m_stereotypeOwningPackage = Singleton<Package, Stereotype, OwningPackagePolicy>(this);
            Singleton<Profile, Stereotype>& getProfileSingleton();
            void restoreReferences() override;
            Stereotype();
        public:
            virtual ~Stereotype();
            ProfilePtr getProfile() const;
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::STEREOTYPE;
            };
    };
}
