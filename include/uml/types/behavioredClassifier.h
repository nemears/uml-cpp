#pragma once

#include "classifier.h"

namespace UML {

    class Behavior;
    class InterfaceRealization;
    typedef UmlPtr<Behavior> BehaviorPtr;

    class BehavioredClassifier : virtual public Classifier {

        protected:
            struct InterfaceRealizationPolicy {
                void elementAdded(InterfaceRealization& el, BehavioredClassifier& me);
                void elementRemoved(InterfaceRealization& el, BehavioredClassifier& me);
            };
            Set<Behavior, BehavioredClassifier> m_ownedBehaviors = Set<Behavior, BehavioredClassifier>(this);
            Singleton<Behavior, BehavioredClassifier> m_classifierBehavior = Singleton<Behavior, BehavioredClassifier>(this);
            Set<InterfaceRealization, BehavioredClassifier, InterfaceRealizationPolicy> m_interfaceRealizations = Set<InterfaceRealization, BehavioredClassifier, InterfaceRealizationPolicy>(this);
            Singleton<Behavior, BehavioredClassifier>& getClassifierBehaviorSingleton();
            BehavioredClassifier();
        public:
            virtual ~BehavioredClassifier();
            Set<Behavior, BehavioredClassifier>& getOwnedBehaviors();
            BehaviorPtr getClassifierBehavior() const;
            void setClassifierBehavior(Behavior& behavior);
            void setClassifierBehavior(Behavior* behavior);
            void setClassifierBehavior(ID id);
            Set<InterfaceRealization, BehavioredClassifier, InterfaceRealizationPolicy>& getInterfaceRealizations();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIORED_CLASSIFIER;
            };
    };
}
