#ifndef LITERAL_UNLIMITED_NATURAL_H
#define LITERAL_UNLIMITED_NATURAL_H

#include "literalSpecification.h"

namespace UML {
    class LiteralUnlimitedNatural : public LiteralSpecification {

        friend class UmlManager;

        private:
            unsigned long m_val;
            bool m_infinite;
            LiteralUnlimitedNatural(); 
        public:
            LiteralUnlimitedNatural(const LiteralUnlimitedNatural& rhs);
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