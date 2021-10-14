#ifndef BEHAVIORED_CLASSIFIER_H
#define BEHAVIORED_CLASSIFIER_H

#include "uml/classifier.h"

namespace UML {

    class Behavior;
    namespace Parsers {
        class SetClassifierBehavior;
    }

    class BehavioredClassifier : virtual public Classifier {

        friend class Parsers::SetClassifierBehavior;

        protected:
            Sequence<Behavior> m_ownedBehaviors = Sequence<Behavior>(this);
            Singleton<Behavior, BehavioredClassifier> m_classifierBehavior = Singleton<Behavior, BehavioredClassifier>(this);
            class RemoveClassifierBehaviorProcedure : public AbstractSingletonProcedure<Behavior, BehavioredClassifier> {
                public:
                    RemoveClassifierBehaviorProcedure(BehavioredClassifier* me) : AbstractSingletonProcedure<Behavior, BehavioredClassifier>(me) {};
                    void operator()(Behavior* el) const override;
            };
            class AddClassifierBehaviorProcedure : public AbstractSingletonProcedure<Behavior, BehavioredClassifier> {
                public:
                    AddClassifierBehaviorProcedure(BehavioredClassifier* me) : AbstractSingletonProcedure<Behavior, BehavioredClassifier>(me) {};
                    void operator()(Behavior* el) const override;
                    void operator()(ID id) const override;
            };
            class AddOwnedBehaviorFunctor : public TemplateAbstractSequenceFunctor<Behavior,BehavioredClassifier> {
                public:
                    AddOwnedBehaviorFunctor(BehavioredClassifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Behavior& el) const override;
                    void operator()(ID id) const override;
            };
            class RemoveOwnedBehaviorFunctor : public TemplateAbstractSequenceFunctor<Behavior,BehavioredClassifier> {
                public:
                    RemoveOwnedBehaviorFunctor(BehavioredClassifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Behavior& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            BehavioredClassifier();
        public:
            BehavioredClassifier(const BehavioredClassifier& classifier);
            virtual ~BehavioredClassifier();
            Sequence<Behavior>& getOwnedBehaviors();
            Behavior* getClassifierBehavior();
            Behavior& getClassifierBehaviorRef();
            ID getClassifierBehaviorID() const;
            bool hasClassifierBehavior() const;
            void setClassifierBehavior(Behavior& behavior);
            void setClassifierBehavior(Behavior* behavior);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIORED_CLASSIFIER;
            };
    };
}

#endif