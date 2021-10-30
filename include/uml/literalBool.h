#ifndef LITERALBOOLH
#define LITERALBOOLH

#include "literalSpecification.h"

namespace UML {
    class LiteralBool : public LiteralSpecification {

        friend class UmlManager;

        protected:
            bool value;
            LiteralBool();
        public:
            LiteralBool(const LiteralBool& rhs);
            bool getValue();
            void setValue(bool val);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_BOOL;
            };
    };
}

#endif