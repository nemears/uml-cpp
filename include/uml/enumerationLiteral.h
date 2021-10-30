#ifndef ENUMERATION_LITERAL_H
#define ENUMERATION_LITERAL_H

#include "instanceSpecification.h"

namespace UML {

    class Enumeration;
    namespace Parsers {
        class SetEnumeration;
    }

    class EnumerationLiteral : public InstanceSpecification {
        
        friend class UmlManager;
        friend class Parsers::SetEnumeration;

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
                    void operator()(ID id) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            EnumerationLiteral();
        public:
            EnumerationLiteral(const EnumerationLiteral& rhs);
            Enumeration* getEnumeration();
            Enumeration& getEnumerationRef();
            ID getEnumerationID() const;
            bool hasEnumeration() const;
            void setEnumeration(Enumeration& enumeration);
            void setEnumeration(Enumeration* enumeration);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ENUMERATION_LITERAL;
            };
    };
}

#endif