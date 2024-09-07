#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralUnlimitedNatural : public LiteralSpecification {
        protected:
            unsigned long m_val;
            bool m_infinite;
            LiteralUnlimitedNatural(std::size_t elementType, AbstractManager& manager); 
        public:
            /** 
             *  NOTE: this will always be initialized to 0
             **/   
            bool isInfinite();
            unsigned long getNumberValue();
            void setNumberValue(unsigned long val);
            void setInfinite();
            typedef TypeInfo<std::tuple<LiteralSpecification>, LiteralUnlimitedNatural> Info;
    };

    template <>
    struct ElementInfo<LiteralUnlimitedNatural> {
        static const bool abstract = false;
        inline static const std::string name {"LiteralUnlimitedNatural"};
        static SetList sets(__attribute__((unused)) LiteralUnlimitedNatural& el) {
            return SetList{};
        }
    };
}
