#ifndef ENUMERATION_LITERAL_H
#define ENUMERATION_LITERAL_H

#include "instanceSpecification.h"

namespace UML {

    class Enumeration;

    class EnumerationLiteral : public InstanceSpecification {
        protected:
            ID m_enumerationID;
            Enumeration* m_enumerationPtr;
        public:
            EnumerationLiteral();
            Enumeration* getEnumeration();
            void setEnumeration(Enumeration* enumeration);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ENUMERATION_LITERAL;
            };
    };
}

#endif