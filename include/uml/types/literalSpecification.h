#pragma once

#include "valueSpecification.h"

namespace UML {
    class LiteralSpecification : public ValueSpecification {
        protected:
            LiteralSpecification();
        public:
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_SPECIFICATION;
            };
    };
}
