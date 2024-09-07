#pragma once

#include "literalSpecification.h"
#include "uml/managers/abstractManager.h"

namespace UML {
    class LiteralReal : public LiteralSpecification {

        protected:
            double m_value = 0;
        public:
            LiteralReal(std::size_t elementType, AbstractManager& manager);
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
