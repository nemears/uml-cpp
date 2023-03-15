#ifndef _UML_ENUMERATION_H_
#define _UML_ENUMERATION_H_

#include "dataType.h"

namespace UML {

    class EnumerationLiteral;

    class Enumeration : public DataType {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class EnumerationLiteral;

        protected:
            CustomOrderedSet<EnumerationLiteral, Enumeration> m_ownedLiterals = CustomOrderedSet<EnumerationLiteral, Enumeration>(this);
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