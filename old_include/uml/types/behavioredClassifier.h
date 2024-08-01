#ifndef _UML_BEHAVIORED_CLASSIFIER_H_
#define _UML_BEHAVIORED_CLASSIFIER_H_

#include "classifier.h"
#include "interfaceRealization.h"

namespace UML {

    class Behavior;
    typedef UmlPtr<Behavior> BehaviorPtr;

    class BehavioredClassifier : virtual public Classifier {

        protected:
            class RemoveInterfaceRealizationPolicy {
                public:
                    static void apply(InterfaceRealization& el, BehavioredClassifier& me);
            };
            class AddInterfaceRealizationPolicy {
                public:
                    static void apply(InterfaceRealization& el, BehavioredClassifier& me);
            };
            CustomSet<Behavior, BehavioredClassifier> m_ownedBehaviors = CustomSet<Behavior, BehavioredClassifier>(this);
            CustomSingleton<Behavior, BehavioredClassifier> m_classifierBehavior = CustomSingleton<Behavior, BehavioredClassifier>(this);
            CustomSet<InterfaceRealization, BehavioredClassifier, AddInterfaceRealizationPolicy, RemoveInterfaceRealizationPolicy> m_interfaceRealizations = CustomSet<InterfaceRealization, BehavioredClassifier, AddInterfaceRealizationPolicy, RemoveInterfaceRealizationPolicy>(this);
            TypedSet<Behavior, BehavioredClassifier>& getClassifierBehaviorSingleton();
            BehavioredClassifier();
        public:
            virtual ~BehavioredClassifier();
            Set<Behavior, BehavioredClassifier>& getOwnedBehaviors();
            BehaviorPtr getClassifierBehavior() const;
            void setClassifierBehavior(Behavior& behavior);
            void setClassifierBehavior(Behavior* behavior);
            void setClassifierBehavior(ID id);
            Set<InterfaceRealization, BehavioredClassifier>& getInterfaceRealizations();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIORED_CLASSIFIER;
            };
    };
}

#endif