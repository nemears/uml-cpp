#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralBool : public LiteralSpecification {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            bool m_value = false;
            LiteralBool();
        public:
            bool getValue();
            void setValue(bool val);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_BOOL;
            };
    };
}
