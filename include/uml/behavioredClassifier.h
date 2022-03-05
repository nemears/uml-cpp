#ifndef _UML_BEHAVIORED_CLASSIFIER_H_
#define _UML_BEHAVIORED_CLASSIFIER_H_

#include "uml/classifier.h"
#include "uml/interfaceRealization.h"

namespace UML {

    class Behavior;
    typedef UmlPtr<Behavior> BehaviorPtr;

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