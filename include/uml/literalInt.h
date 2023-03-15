#ifndef _UML_LITERAL_INT_H_
#define _UML_LITERAL_INT_H_

#include "literalSpecification.h"

namespace UML {
    class LiteralInt : public LiteralSpecification {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            int m_value = 0; // TODO change to long so we can hold any number?
            LiteralInt();
        public:
            int getValue();
            void setValue(int val);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_INT;
            };
    };
}

#endif