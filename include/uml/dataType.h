#ifndef DATATYPE_H
#define DATATYPE_H

#include "classifier.h"

namespace UML {
    class DataType : public Classifier {
        friend class UmlManager;
        protected:
            Sequence<Property> m_ownedAttribute = Sequence<Property>(this);
            Sequence<Operation> m_ownedOperation = Sequence<Operation>(this);
            class AddOwnedAttributeFunctor : public TemplateAbstractSequenceFunctor<Property,DataType> {
                public:
                    AddOwnedAttributeFunctor(DataType* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class RemoveOwnedAttributeFunctor : public TemplateAbstractSequenceFunctor<Property,DataType> {
                public:
                    RemoveOwnedAttributeFunctor(DataType* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class AddOwnedOperationFunctor : public TemplateAbstractSequenceFunctor<Operation,DataType> {
                public:
                    AddOwnedOperationFunctor(DataType* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Operation& el) const override;
            };
            class RemoveOwnedOperationFunctor : public TemplateAbstractSequenceFunctor<Operation,DataType> {
                public:
                    RemoveOwnedOperationFunctor(DataType* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Operation& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            DataType();
        public:
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