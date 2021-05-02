#ifndef CLASSH
#define CLASSH
#include <list>
#include "structuredClassifier.h"

namespace UML{

    /**
     * This is a forward declaration of Operation because the method field in Operation is of type Behavior
     * which inherits from this class (Class), so we just declare operation here and include the header in class.cpp
     **/
    class Operation;

    /**
     * Definition of class here
     **/
    class Class : public StructuredClassifier {
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
        public:
            Class();
            ~Class();
            Sequence<Operation>& getOperations();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif