#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralReal : public LiteralSpecification {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            double m_value = 0;
            LiteralReal();
        public:
            double getValue();
            void setValue(double val);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_REAL;
            };
    };
}
