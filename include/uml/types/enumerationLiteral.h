#ifndef _UML_ENUMERATION_LITERAL_H_
#define _UML_ENUMERATION_LITERAL_H_

#include "instanceSpecification.h"

namespace UML {

    class Enumeration;

    typedef UmlPtr<Enumeration> EnumerationPtr;

    class EnumerationLiteral : public InstanceSpecification {
        
        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Enumeration;

        protected:
            CustomSingleton<Enumeration, EnumerationLiteral> m_enumeration = CustomSingleton<Enumeration, EnumerationLiteral>(this);
            TypedSet<Enumeration, EnumerationLiteral>& getEnumerationSingleton();
            EnumerationLiteral();
        public:
            virtual ~EnumerationLiteral();
            EnumerationPtr getEnumeration() const;
            void setEnumeration(Enumeration& enumeration);
            void setEnumeration(Enumeration* enumeration);
            void setEnumeration(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ENUMERATION_LITERAL;
            };
    };
}

#endif