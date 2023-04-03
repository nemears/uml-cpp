#ifndef _UML_STEREOTYPE_H_
#define _UML_STEREOTYPE_H_

#include "class.h"
#include "profile.h"

namespace UML {

    class Profile;
    typedef UmlPtr<Profile> ProfilePtr;

    class Stereotype : public Class {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Profile;

        private:

            class SetOwningPackagePolicy {
                public:
                    void apply(Package& el, Stereotype& me);
            };

            class RemoveOwningPackagePolicy {
                public:
                    void apply(Package& package, Stereotype& me);
            };

            CustomSingleton<Profile, Stereotype> m_profile = CustomSingleton<Profile, Stereotype>(this);
            CustomSingleton<Package, Stereotype, SetOwningPackagePolicy, RemoveOwningPackagePolicy> m_stereotypeOwningPackage = CustomSingleton<Package, Stereotype, SetOwningPackagePolicy, RemoveOwningPackagePolicy>(this);
            TypedSet<Profile, Stereotype>& getProfileSingleton();
            void restoreReferences() override;
            Stereotype();
        public:
            virtual ~Stereotype();
            ProfilePtr getProfile() const;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::STEREOTYPE;
            };
    };
}

#endif