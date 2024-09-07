#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralBool : public LiteralSpecification {

        protected:
            bool m_value = false;
            LiteralBool(std::size_t elementType, AbstractManager& manager);
        public:
            bool getValue();
            void setValue(bool val);
            typedef TypeInfo<std::tuple<LiteralSpecification>, LiteralBool> Info;
    };

    template <>
    struct ElementInfo<LiteralBool> {
        static const bool abstract = false;
        inline static const std::string name {"LiteralBool"};
        static SetList sets(__attribute__((unused)) LiteralBool& el) {
            return SetList {};
        }
    };
}
