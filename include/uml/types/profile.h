#pragma once 

#include "package.h"
#include "uml/managers/abstractManager.h"

namespace UML {
    class Profile : public Package {

        friend class Stereotype;

        public:
            Profile(std::size_t elementType, AbstractManager& manager);
            typedef TypeInfo<std::tuple<Package>, Profile> Info;
    };

    template<>
    struct ElementInfo<Profile> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Profile"; }
    };
}
