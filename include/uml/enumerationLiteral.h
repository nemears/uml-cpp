#ifndef _UML_ENUMERATION_LITERAL_H_
#define _UML_ENUMERATION_LITERAL_H_

#include "instanceSpecification.h"

namespace UML {

    class Enumeration;

    typedef UmlPtr<Enumeration> EnumerationPtr;

    class EnumerationLiteral : public InstanceSpecification {
        
        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        friend class Enumeration;

        protected:
            Singleton<Enumeration, EnumerationLiteral> m_enumeration = Singleton<Enumeration, EnumerationLiteral>(this);
            Set<Enumeration, EnumerationLiteral>& getEnumerationSingleton();
            void init();
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