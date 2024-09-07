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
    struct ElementInfo<Profile> {
        static const bool abstract = false;
        inline static std::string name {"Profile"};
        static SetList sets(__attribute__((unused)) Profile& el) {
            return SetList{};
        }
    };
}
