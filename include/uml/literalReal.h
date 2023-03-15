#ifndef _UML_LITERAL_REAL_H_
#define _UML_LITERAL_REAL_H_

#include "literalSpecification.h"

namespace UML {
    class LiteralReal : public LiteralSpecification {

        template <typename AccessPolicy, typename SerializationPolicy> friend class Manager;

        protected:
            double m_value = 0;
            LiteralReal();
        public:
            double getValue();
            void setValue(double val);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_REAL;
            };
    };
}

#endif