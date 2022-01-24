#ifndef BEHAVIORED_CLASSIFIER_H
#define BEHAVIORED_CLASSIFIER_H

#include "uml/classifier.h"

namespace UML {

    class Behavior;

    class BehavioredClassifier : virtual public Classifier {

        protected:
            Set<Behavior, BehavioredClassifier> m_ownedBehaviors = Set<Behavior, BehavioredClassifier>(this);
            Singleton<Behavior, BehavioredClassifier> m_classifierBehavior = Singleton<Behavior, BehavioredClassifier>(this);
            Set<InterfaceRealization, BehavioredClassifier> m_interfaceRealizations = Set<InterfaceRealization, BehavioredClassifier>(this);
            class RemoveInterfaceRealizationFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveInterfaceRealizationFunctor(Element* el) : SetFunctor(el) {};
            };
            class AddInterfaceRealizationFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    AddInterfaceRealizationFunctor(Element* el) : SetFunctor(el) {};
            };
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
            Set<InterfaceRealization, BehavioredClassifier>& getInterfaceRealizations();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIORED_CLASSIFIER;
            };
    };
}

#endif