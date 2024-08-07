#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralInt : public LiteralSpecification {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            int m_value = 0; // TODO change to long so we can hold any number?
            LiteralInt();
        public:
            int getValue();
            void setValue(int val);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_INT;
            };
    };
}
