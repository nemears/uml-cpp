#pragma once

#include "class.h"
#include "uml/managers/typeInfo.h"
#include "uml/types/element.h"

namespace UML {

    class Profile;
    typedef UmlPtr<Profile> ProfilePtr;

    class Stereotype : public Class {

        friend class Profile;
        friend struct ElementInfo<Stereotype>;

        protected:

            struct OwningPackagePolicy {
                void elementAdded(Package& el, Stereotype& me);
                void elementRemoved(Package& el, Stereotype& me);
            };


            Singleton<Profile, Stereotype> m_profile = Singleton<Profile, Stereotype>(this);
            Singleton<Package, Stereotype, OwningPackagePolicy> m_stereotypeOwningPackage = Singleton<Package, Stereotype, OwningPackagePolicy>(this);
            Singleton<Profile, Stereotype>& getProfileSingleton();
            Stereotype(std::size_t elementType, AbstractManager& manager);
        public:
            ProfilePtr getProfile() const;
            typedef TypeInfo<std::tuple<Class>, Stereotype> Info;
    };

    template <>
    struct ElementInfo<Stereotype> {
        static const bool abstract = false;
        inline static const std::string name {"Stereotype"};
        static SetList sets(Stereotype& el) {
            return SetList {
                makeSetPair("profile", el.m_profile),
                makeSetPair("owningPackage", el.m_owningPackage)
            };
        }
    };
}
