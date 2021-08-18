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
            Sequence<Operation> m_ownedOperations = Sequence<Operation>(this);
            Sequence<Classifier> m_nestedClassifiers = Sequence<Classifier>(this);
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
            class AddNestedClassifierFunctor : public AbstractSequenceFunctor {
                public:
                    AddNestedClassifierFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveNestedClassifierFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveNestedClassifierFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
        public:
            Class();
            virtual ~Class();
            Class(const Class& clazz);
            Sequence<Operation>& getOwnedOperations();
            Sequence<Classifier>& getNestedClassifiers();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CLASS;
            };
    };
}

#endif