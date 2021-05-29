#ifndef LITERALBOOLH
#define LITERALBOOLH

#include "literalSpecification.h"
#include "primitiveType.h"

namespace UML {
    class LiteralBool : public LiteralSpecification {
        protected:
            bool value;
        public:
            bool getValue();
            void setValue(bool val);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::LITERAL_BOOL;
            };
    };
}

#endif