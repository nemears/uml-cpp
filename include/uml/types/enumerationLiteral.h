#pragma once

#include "instanceSpecification.h"
#include "uml/managers/typeInfo.h"
#include "uml/types/element.h"

namespace UML {

    class Enumeration;

    typedef UmlPtr<Enumeration> EnumerationPtr;

    class EnumerationLiteral : public InstanceSpecification {
        
        friend class Enumeration;
        friend struct ElementInfo<EnumerationLiteral>;
        
        protected:
            Singleton<Enumeration, EnumerationLiteral> m_enumeration = Singleton<Enumeration, EnumerationLiteral>(this);
            Singleton<Enumeration, EnumerationLiteral>& getEnumerationSingleton();
        public:
            EnumerationLiteral(std::size_t elementType, AbstractManager& manager);
            EnumerationPtr getEnumeration() const;
            void setEnumeration(Enumeration& enumeration);
            void setEnumeration(EnumerationPtr enumeration);
            void setEnumeration(ID id);
            typedef TypeInfo<std::tuple<InstanceSpecification>, EnumerationLiteral> Info;
    };

    template <>
    struct ElementInfo<EnumerationLiteral> : public DefaultInfo {
        static const bool abstract = false;
        static constexpr std::string_view name {"EnumerationLiteral"};
        static SetList sets(EnumerationLiteral& el) {
            return SetList {
                makeSetPair("enumeration", el.m_enumeration)
            };
        }
    };
}
