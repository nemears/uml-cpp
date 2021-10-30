#ifndef LITERALINTH
#define LITERALINTH

#include "literalSpecification.h"

namespace UML {
    class LiteralInt : public LiteralSpecification {

        friend class UmlManager;

        protected:
            int value = 0; // TODO change to long so we can hold any number?
            LiteralInt();
        public:
            LiteralInt(const LiteralInt& rhs);
            int getValue();
            void setValue(int val);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_INT;
            };
    };
}

#endif