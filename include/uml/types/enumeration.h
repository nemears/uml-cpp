#pragma once

#include "dataType.h"

namespace UML {

    class EnumerationLiteral;

    class Enumeration : public DataType {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class EnumerationLiteral;

        protected:
            OrderedSet<EnumerationLiteral, Enumeration> m_ownedLiterals = OrderedSet<EnumerationLiteral, Enumeration>(this);
            Enumeration();
        public:
            virtual ~Enumeration();
            OrderedSet<EnumerationLiteral, Enumeration>& getOwnedLiterals();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ENUMERATION;
            };
    };
}
