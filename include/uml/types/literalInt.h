#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralInt : public LiteralSpecification {
        protected:
            int m_value = 0; // TODO change to long so we can hold any number?
        public:
            LiteralInt(std::size_t elementType, AbstractManager& manager);
            int getValue();
            void setValue(int val);
            typedef TypeInfo<std::tuple<LiteralSpecification>, LiteralInt> Info;
    };

    template <>
    struct ElementInfo<LiteralInt> {
        static const bool abstract = false;
        inline static std::string name {""};
        static SetList sets(__attribute__((unused)) LiteralInt& el) {
            return SetList{};
        }
    };
}
