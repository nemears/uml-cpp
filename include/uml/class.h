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
            class AddOwnedOperationFunctor : public TemplateAbstractSequenceFunctor<Operation,Class> {
                public:
                    AddOwnedOperationFunctor(Class* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Operation& el) const override;
                    void operator()(ID id) const override;
            };
            class RemoveOwnedOperationFunctor : public TemplateAbstractSequenceFunctor<Operation,Class> {
                public:
                    RemoveOwnedOperationFunctor(Class* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Operation& el) const override;
            };
            class ClassAddOwnedAttributeFunctor : public TemplateAbstractSequenceFunctor<Property,Class> {
                public:
                    ClassAddOwnedAttributeFunctor(Class* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class ClassRemoveOwnedAttributeFunctor : public TemplateAbstractSequenceFunctor<Property,Class> {
                public:
                    ClassRemoveOwnedAttributeFunctor(Class* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class AddNestedClassifierFunctor : public TemplateAbstractSequenceFunctor<Classifier,Class> {
                public:
                    AddNestedClassifierFunctor(Class* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Classifier& el) const override;
                    void operator()(ID id) const override;
            };
            class RemoveNestedClassifierFunctor : public TemplateAbstractSequenceFunctor<Classifier,Class> {
                public:
                    RemoveNestedClassifierFunctor(Class* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Classifier& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            Class();
        public:
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