#ifndef DATATYPE_H
#define DATATYPE_H

#include "classifier.h"

namespace UML {
    class DataType : public Classifier {
        friend class UmlManager;
        protected:
            Sequence<Property> m_ownedAttribute;
            Sequence<Operation> m_ownedOperation;
            void setManager(UmlManager* manager) override;
            class AddOwnedAttributeFunctor : public AbstractSequenceFunctor {
                public:
                    AddOwnedAttributeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveOwnedAttributeFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveOwnedAttributeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddOwnedOperationFunctor : public AbstractSequenceFunctor {
                public:
                    AddOwnedOperationFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveOwnedOperationFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveOwnedOperationFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            DataType();
            ~DataType();
            DataType(const DataType& data);
            Sequence<Property>& getOwnedAttribute();
            Sequence<Operation>& getOwnedOperation();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DATA_TYPE;
            };
    };
}

#endif