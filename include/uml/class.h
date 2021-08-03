#ifndef CLASSH
#define CLASSH
#include <list>
#include "structuredClassifier.h"
#include "behavioredClassifier.h"

namespace UML{

    class Operation;

    class Class : public StructuredClassifier , public BehavioredClassifier {
        friend class UmlManager;
        protected:
            Sequence<Operation> m_operations;
            class AddOperationFunctor : public AbstractSequenceFunctor {
                public:
                    AddOperationFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveOperationFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveOperationFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class ClassAddOwnedAttributeFunctor : public AbstractSequenceFunctor {
                public:
                    ClassAddOwnedAttributeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class ClassRemoveOwnedAttributeFunctor : public AbstractSequenceFunctor {
                public:
                    ClassRemoveOwnedAttributeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            void setManager(UmlManager* manager) override;
        public:
            Class();
            virtual ~Class();
            Class(const Class& clazz);
            Sequence<Operation>& getOperations();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CLASS;
            };
    };
}

#endif