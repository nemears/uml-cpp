#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralNull : public LiteralSpecification {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            LiteralNull();
        public:
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_NULL;
            };
    };
}
