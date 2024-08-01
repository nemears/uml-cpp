#ifndef _UML_LITERAL_NULL_H_
#define _UML_LITERAL_NULL_H_

#include "literalSpecification.h"

namespace UML {
    class LiteralNull : public LiteralSpecification {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            LiteralNull();
        public:
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_NULL;
            };
    };
}

#endif