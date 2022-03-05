#ifndef _UML_LITERAL_SPECIFICATION_H_
#define _UML_LITERAL_SPECIFICATION_H_

#include "valueSpecification.h"

namespace UML {
    class LiteralSpecification : public ValueSpecification {
        protected:
            LiteralSpecification();
        public:
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_SPECIFICATION;
            };
    };
}

#endif