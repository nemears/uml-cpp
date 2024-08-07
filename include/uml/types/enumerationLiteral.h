#pragma once

#include "instanceSpecification.h"

namespace UML {

    class Enumeration;

    typedef UmlPtr<Enumeration> EnumerationPtr;

    class EnumerationLiteral : public InstanceSpecification {
        
        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Enumeration;

        protected:
            Singleton<Enumeration, EnumerationLiteral> m_enumeration = Singleton<Enumeration, EnumerationLiteral>(this);
            Singleton<Enumeration, EnumerationLiteral>& getEnumerationSingleton();
            EnumerationLiteral();
        public:
            virtual ~EnumerationLiteral();
            EnumerationPtr getEnumeration() const;
            void setEnumeration(Enumeration& enumeration);
            void setEnumeration(Enumeration* enumeration);
            void setEnumeration(ID id);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ENUMERATION_LITERAL;
            };
    };
}
