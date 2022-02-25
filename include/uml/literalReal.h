#ifndef LITERALREALH
#define LITERALREALH

#include "literalSpecification.h"

namespace UML {
    class LiteralReal : public LiteralSpecification {

        friend class UmlManager;

        protected:
            double m_value = 0;
            LiteralReal();
        public:
            LiteralReal(const LiteralReal& rhs);
            double getValue();
            void setValue(double val);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_REAL;
            };
    };
}

#endif