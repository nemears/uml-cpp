#ifndef BEHAVIORED_CLASSIFIER_H
#define BEHAVIORED_CLASSIFIER_H

#include "uml/classifier.h"

namespace UML {

    class Behavior;

    class BehavioredClassifier : virtual public Classifier {

        protected:
            Set<Behavior, BehavioredClassifier> m_ownedBehaviors = Set<Behavior, BehavioredClassifier>(this);
            Singleton<Behavior, BehavioredClassifier> m_classifierBehavior = Singleton<Behavior, BehavioredClassifier>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<Behavior, BehavioredClassifier>& getClassifierBehaviorSingleton();
            void init();
            void copy(const BehavioredClassifier& rhs);
            BehavioredClassifier();
        public:
            BehavioredClassifier(const BehavioredClassifier& rhs);
            virtual ~BehavioredClassifier();
            Set<Behavior, BehavioredClassifier>& getOwnedBehaviors();
            Behavior* getClassifierBehavior();
            Behavior& getClassifierBehaviorRef();
            ID getClassifierBehaviorID() const;
            bool hasClassifierBehavior() const;
            void setClassifierBehavior(Behavior& behavior);
            void setClassifierBehavior(Behavior* behavior);
            void setClassifierBehavior(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIORED_CLASSIFIER;
            };
    };
}

#endif