#ifndef LITERALSTRINGH
#define LITERALSTRINGH

#include "literalSpecification.h"

namespace UML {
    class LiteralString : public LiteralSpecification {

        friend class UmlManager;

        protected:
            std::string value;
            LiteralString();
        public:
            LiteralString(const LiteralString& rhs);
            std::string getValue();
            void setValue(std::string val);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_STRING;
            };
    };
}

#endif