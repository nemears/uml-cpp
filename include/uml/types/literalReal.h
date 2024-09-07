#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralReal : public LiteralSpecification {

        protected:
            double m_value = 0;
            LiteralReal();
        public:
            double getValue();
            void setValue(double val);
            typedef TypeInfo<std::tuple<LiteralSpecification>, LiteralReal> Info;
    };

    template <>
    struct ElementInfo<LiteralReal> {
        static const bool abstract = false;
        inline static const std::string name{"LiteralReal"};
        static SetList sets(__attribute__((unused)) LiteralReal& el) {
            return SetList {};
        }
    };
}
