#ifndef _UML_LITERAL_BOOL_H_
#define _UML_LITERAL_BOOL_H_

#include "literalSpecification.h"

namespace UML {
    class LiteralBool : public LiteralSpecification {

        friend class UmlManager;

        protected:
            bool m_value = false;
            LiteralBool();
        public:
            bool getValue();
            void setValue(bool val);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_BOOL;
            };
    };
}

#endif