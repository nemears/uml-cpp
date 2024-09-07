#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralNull : public LiteralSpecification {
        protected:
            LiteralNull(std::size_t elementType, AbstractManager& manager);
        public:
            typedef TypeInfo<std::tuple<LiteralSpecification>, LiteralNull> Info;
    };

    template <>
    struct ElementInfo<LiteralNull> {
        static const bool abstract = false;
        inline static const std::string name {"LiteralNull"};
        static SetList sets(__attribute__((unused)) LiteralNull& el) {
            return SetList{};
        }
    };
}
