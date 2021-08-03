#ifndef BEHAVIORED_CLASSIFIER_H
#define BEHAVIORED_CLASSIFIER_H

#include "uml/classifier.h"

namespace UML {

    class Behavior;

    class BehavioredClassifier : virtual public Classifier {
        protected:
            Sequence<Behavior> m_ownedBehaviors;
            ID m_classifierBehaviorID;
            Behavior* m_classifierBehaviorPtr;
            class AddOwnedBehaviorFunctor : public AbstractSequenceFunctor {
                public:
                    AddOwnedBehaviorFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveOwnedBehaviorFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveOwnedBehaviorFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            void setManager(UmlManager* manager) override;
        public:
            BehavioredClassifier();
            BehavioredClassifier(const BehavioredClassifier& classifier);
            virtual ~BehavioredClassifier();
            Sequence<Behavior>& getOwnedBehaviors();
            Behavior* getClassifierBehavior();
            void setClassifierBehavior(Behavior* behavior);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIORED_CLASSIFIER;
            };
    };
}

#endif