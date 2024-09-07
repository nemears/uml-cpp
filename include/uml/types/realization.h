#pragma once

#include "abstraction.h"

namespace UML {
    /**
     * Realization is a specialized Abstraction dependency between two sets of NamedElements, one representing a 
     * specification (the supplier) and the other representing an implementation of that specification (the client).
     **/
    class Realization : public Abstraction {
        protected:
            Realization(std::size_t elementType);
        public:
            typedef TypeInfo<std::tuple<Abstraction>, Realization> Info;
    };

    template <>
    struct ElementInfo<Realization> {
        static const bool abstract = false;
        inline static const std::string name {"Realization"};
        static SetList sets(__attribute__((unused)) Realization& el) {
            return SetList{};
        }
    };
}
