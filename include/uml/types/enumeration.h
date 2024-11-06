#pragma once

#include "dataType.h"
#include "uml/types/element.h"

namespace UML {

    class EnumerationLiteral;

    class Enumeration : public DataType {

        friend class EnumerationLiteral;

        protected:
            OrderedSet<EnumerationLiteral, Enumeration> m_ownedLiterals = OrderedSet<EnumerationLiteral, Enumeration>(this);
        public:
            Enumeration(std::size_t elementType, AbstractManager& manager);
            OrderedSet<EnumerationLiteral, Enumeration>& getOwnedLiterals();
            typedef TypeInfo<std::tuple<DataType>, Enumeration> Info;
    };

    template <>
    struct ElementInfo<Enumeration> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Enumeration"; }
        static SetList sets(Enumeration& el) {
            return SetList {
                makeSetPair("ownedLiterals", el.getOwnedLiterals())
            };
        }
    };
}
