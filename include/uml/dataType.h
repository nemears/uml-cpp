#ifndef DATATYPE_H
#define DATATYPE_H

#include "classifier.h"

namespace UML {
    class DataType : public Classifier {
        protected:
            Sequence<Property>* m_ownedAttribute;
            Sequence<Operation>* m_ownedOperation;
            class AddOwnedAttributeFunctor : public AbstractSequenceFunctor {
                public:
                    AddOwnedAttributeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            DataType();
            ~DataType();
            DataType(const DataType& data);
            Sequence<Property>& getOwnedAttribute();
            Sequence<Operation>& getOwnedOperation();
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif