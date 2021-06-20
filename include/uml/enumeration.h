#ifndef ENUMERATION_H
#define ENUMERATION_H

#include "dataType.h"
#include "enumerationLiteral.h"

namespace UML {
    class Enumeration : public DataType {
        protected:
            Sequence<EnumerationLiteral> m_ownedLiteral;
            class AddOwnedLiteralFunctor : public AbstractSequenceFunctor {
                public:
                    AddOwnedLiteralFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveOwnedLiteralFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveOwnedLiteralFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            Enumeration();
            Enumeration(const Enumeration& enumeration);
            Sequence<EnumerationLiteral>& getOwnedLiteral();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ENUMERATION;
            };
    };
}

#endif