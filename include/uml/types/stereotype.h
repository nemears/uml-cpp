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


            ReadOnlySingleton<Profile, Stereotype> m_profile = ReadOnlySingleton<Profile, Stereotype>(this);
            Singleton<Package, Stereotype, OwningPackagePolicy> m_stereotypeOwningPackage = Singleton<Package, Stereotype, OwningPackagePolicy>(this);
            ReadOnlySingleton<Profile, Stereotype>& getProfileSingleton();
        public:
            Stereotype(std::size_t elementType, AbstractManager& manager);
            ProfilePtr getProfile() const;
            typedef TypeInfo<std::tuple<Class>, Stereotype> Info;
    };

    template <>
    struct ElementInfo<Stereotype> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Stereotype"; }
        static SetList sets(Stereotype& el) {
            return SetList {
                makeSetPair("profile", el.m_profile),
                makeSetPair("owningPackage", el.m_stereotypeOwningPackage)
            };
        }
    };
}
