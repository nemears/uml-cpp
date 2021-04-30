#ifndef LITERALSTRINGH
#define LITERALSTRINGH

#include "literalSpecification.h"
#include "primitiveType.h"
#include <string>

namespace UML {
    class LiteralString : public LiteralSpecification {
        protected:
            std::string value;
        public:
            std::string getValue();
            void setValue(std::string val);
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif