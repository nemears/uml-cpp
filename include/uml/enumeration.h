#ifndef _UML_ENUMERATION_H_
#define _UML_ENUMERATION_H_

#include "dataType.h"

namespace UML {

    class EnumerationLiteral;

    class Enumeration : public DataType {

        friend class UmlManager;
        friend class EnumerationLiteral;

        protected:
            OrderedSet<EnumerationLiteral, Enumeration> m_ownedLiterals = OrderedSet<EnumerationLiteral, Enumeration>(this);
            Set<EnumerationLiteral, Enumeration>& getOwnedLiteralsSet();
            void init();
            Enumeration();
        public:
            virtual ~Enumeration();
            OrderedSet<EnumerationLiteral, Enumeration>& getOwnedLiterals();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ENUMERATION;
            };
    };
}

#endif