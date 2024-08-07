#pragma once

#include "literalSpecification.h"

namespace UML {
    class LiteralUnlimitedNatural : public LiteralSpecification {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        private:
            unsigned long m_val;
            bool m_infinite;
            LiteralUnlimitedNatural(); 
        public:
            /** 
             *  NOTE: this will always be initialized to 0
             **/   
            bool isInfinite();
            unsigned long getNumberValue();
            void setNumberValue(unsigned long val);
            void setInfinite();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_UNLIMITED_NATURAL;
            };
    };
}
