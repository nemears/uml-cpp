#ifndef _UML_LITERAL_UNLIMITED_NATURAL_H_
#define _UML_LITERAL_UNLIMITED_NATURAL_H_

#include "literalSpecification.h"

namespace UML {
    class LiteralUnlimitedNatural : public LiteralSpecification {

        template <typename AccessPolicy, typename SerializationPolicy> friend class Manager;

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
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_UNLIMITED_NATURAL;
            };
    };
}

#endif