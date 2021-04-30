#ifndef LITERAL_SPECIFICATION_H
#define LITERAL_SPECIFICATION_H

#include "valueSpecification.h"

namespace UML {
    class LiteralSpecification : public ValueSpecification {
        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif