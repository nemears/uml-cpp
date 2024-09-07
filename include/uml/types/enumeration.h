#pragma once

#include "dataType.h"
#include "uml/types/element.h"

namespace UML {

    class EnumerationLiteral;

    class Enumeration : public DataType {

        friend class EnumerationLiteral;

        protected:
            OrderedSet<EnumerationLiteral, Enumeration> m_ownedLiterals = OrderedSet<EnumerationLiteral, Enumeration>(this);
            Enumeration(std::size_t elementType, AbstractManager& manager);
        public:
            OrderedSet<EnumerationLiteral, Enumeration>& getOwnedLiterals();
            typedef TypeInfo<std::tuple<DataType>, Enumeration> Info;
    };

    template <>
    struct ElementInfo<Enumeration> {
        static const bool abstract = false;
        inline static const std::string name {"Enumeration"};
        static SetList sets(Enumeration& el) {
            return SetList {
                makeSetPair("ownedLiterals", el.getOwnedLiterals())
            };
        }
    };
}
