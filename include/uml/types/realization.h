#pragma once

#include "abstraction.h"

namespace UML {
    /**
     * Realization is a specialized Abstraction dependency between two sets of NamedElements, one representing a 
     * specification (the supplier) and the other representing an implementation of that specification (the client).
     **/
    class Realization : public Abstraction {
        public:
            Realization(std::size_t elementType, AbstractManager& manager);
            typedef TypeInfo<std::tuple<Abstraction>, Realization> Info;
    };

    template <>
    struct ElementInfo<Realization> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Realization"; }
    };
}
