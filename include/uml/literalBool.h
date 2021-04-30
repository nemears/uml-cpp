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
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif