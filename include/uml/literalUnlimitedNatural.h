#ifndef LITERAL_UNLIMITED_NATURAL_H
#define LITERAL_UNLIMITED_NATURAL_H

#include "literalSpecification.h"

namespace UML {
    class LiteralUnlimitedNatural : public LiteralSpecification {
        private:
        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_UNLIMITED_NATURAL;
            };
    };
}

#endif