#ifndef ENUMERATION_LITERAL_H
#define ENUMERATION_LITERAL_H

#include "instanceSpecification.h"

namespace UML {

    class Enumeration;

    class EnumerationLiteral : public InstanceSpecification {
        
        friend class UmlManager;
        friend class Enumeration;

        protected:
            Singleton<Enumeration, EnumerationLiteral> m_enumeration = Singleton<Enumeration, EnumerationLiteral>(this);
            Set<Enumeration, EnumerationLiteral>& getEnumerationSingleton();
            void init();
            void copy(const EnumerationLiteral& rhs);
            EnumerationLiteral();
        public:
            EnumerationLiteral(const EnumerationLiteral& rhs);
            virtual ~EnumerationLiteral();
            Enumeration* getEnumeration();
            Enumeration& getEnumerationRef();
            ID getEnumerationID() const;
            bool hasEnumeration() const;
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