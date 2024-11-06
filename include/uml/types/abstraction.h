#pragma once

#include "dependency.h"
#include "uml/managers/abstractManager.h"

namespace UML {
    /**
     * An Abstraction is a Dependency that relates two NamedElements or sets of NamedElements that represent the same 
     * concept at different levels of abstraction or from different viewpoints.
     **/
    class Abstraction : public Dependency {
        public:
            Abstraction(std::size_t elementType, AbstractManager& manager);
            typedef TypeInfo<std::tuple<Dependency>, Abstraction> Info;
    };

    template <>
    struct ElementInfo<Abstraction> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Abstraction"; }
        static SetList sets(__attribute__((unused)) Abstraction& el) {
            return SetList {};
        }
    };
}
