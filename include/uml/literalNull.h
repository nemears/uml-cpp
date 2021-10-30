#ifndef LITERAL_NULL_H
#define LITERAL_NULL_H

#include "literalSpecification.h"

namespace UML {
    class LiteralNull : public LiteralSpecification {

        friend class UmlManager;

        protected:
            LiteralNull();
        public:
            LiteralNull(const LiteralNull& rhs);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_NULL;
            };
    };
}

#endif