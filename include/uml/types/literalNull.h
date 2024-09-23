#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralNull : public LiteralSpecification {
        public:
            LiteralNull(std::size_t elementType, AbstractManager& manager);
            typedef TypeInfo<std::tuple<LiteralSpecification>, LiteralNull> Info;
    };

    template <>
    struct ElementInfo<LiteralNull> : public DefaultInfo {
        static const bool abstract = false;
        static constexpr std::string_view name {"LiteralNull"};
    };
}
