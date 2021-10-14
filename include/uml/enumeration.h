#ifndef ENUMERATION_H
#define ENUMERATION_H

#include "dataType.h"

namespace UML {

    class EnumerationLiteral;

    class Enumeration : public DataType {

        friend class UmlManager;

        protected:
            Sequence<EnumerationLiteral> m_ownedLiterals = Sequence<EnumerationLiteral>(this);
            class AddOwnedLiteralFunctor : public TemplateAbstractSequenceFunctor<EnumerationLiteral,Enumeration> {
                public:
                    AddOwnedLiteralFunctor(Enumeration* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(EnumerationLiteral& el) const override;
                    void operator()(ID id) const override;
            };
            class RemoveOwnedLiteralFunctor : public TemplateAbstractSequenceFunctor<EnumerationLiteral,Enumeration> {
                public:
                    RemoveOwnedLiteralFunctor(Enumeration* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(EnumerationLiteral& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Enumeration();
        public:
            Enumeration(const Enumeration& enumeration);
            Sequence<EnumerationLiteral>& getOwnedLiterals();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ENUMERATION;
            };
    };
}

#endif