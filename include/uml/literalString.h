#ifndef LITERALSTRINGH
#define LITERALSTRINGH

#include "literalSpecification.h"

namespace UML {
    class LiteralString : public LiteralSpecification {
        protected:
            std::string value;
        public:
            std::string getValue();
            void setValue(std::string val);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_STRING;
            };
    };
}

#endif