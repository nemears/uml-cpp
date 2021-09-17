#ifndef ENUMERATION_LITERAL_H
#define ENUMERATION_LITERAL_H

#include "instanceSpecification.h"

namespace UML {

    class Enumeration;

    class EnumerationLiteral : public InstanceSpecification {
        
        friend class UmlManager;

        protected:
            Singleton<Enumeration, EnumerationLiteral> m_enumeration = Singleton<Enumeration, EnumerationLiteral>(this);
            class RemoveEnumerationProcedure : public AbstractSingletonProcedure<Enumeration, EnumerationLiteral> {
                public:
                    RemoveEnumerationProcedure(EnumerationLiteral* me) : AbstractSingletonProcedure<Enumeration, EnumerationLiteral>(me) {};
                    void operator()(Enumeration* el) const override;
            };
            class AddEnumerationProcedure : public AbstractSingletonProcedure<Enumeration, EnumerationLiteral> {
                public:
                    AddEnumerationProcedure(EnumerationLiteral* me) : AbstractSingletonProcedure<Enumeration, EnumerationLiteral>(me) {};
                    void operator()(Enumeration* el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            EnumerationLiteral();
        public:
            EnumerationLiteral(const EnumerationLiteral& rhs);
            Enumeration* getEnumeration();
            Enumeration& getEnumerationRef();
            bool hasEnumeration() const;
            void setEnumeration(Enumeration& enumeration);
            void setEnumeration(Enumeration* enumeration);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ENUMERATION_LITERAL;
            };
    };
}

#endif