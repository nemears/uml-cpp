#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class Package;

    template <class ManagerPolicy>
    class Profile : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Profile, EGM::TemplateTypeList<Package>>;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Profile);
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Profile> {
        static std::string name() { return "Profile"; }
    };
}
