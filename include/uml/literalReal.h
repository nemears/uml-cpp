#ifndef LITERALREALH
#define LITERALREALH

#include "literalSpecification.h"
#include "primitiveType.h"

namespace UML {
    class LiteralReal : public LiteralSpecification {
        protected:
            double value;
        public:
            double getValue();
            void setValue(double val);
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif