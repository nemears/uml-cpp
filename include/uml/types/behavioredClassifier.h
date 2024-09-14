#pragma once

#include "classifier.h"
#include "uml/types/element.h"

namespace UML {

    class Behavior;
    class InterfaceRealization;
    typedef UmlPtr<Behavior> BehaviorPtr;

    class BehavioredClassifier : virtual public Classifier {

        friend struct ElementInfo<BehavioredClassifier>;

        protected:
            struct InterfaceRealizationPolicy {
                void elementAdded(InterfaceRealization& el, BehavioredClassifier& me);
                void elementRemoved(InterfaceRealization& el, BehavioredClassifier& me);
            };
            IndexableSet<Behavior, BehavioredClassifier> m_ownedBehaviors = IndexableSet<Behavior, BehavioredClassifier>(this);
            Singleton<Behavior, BehavioredClassifier> m_classifierBehavior = Singleton<Behavior, BehavioredClassifier>(this);
            Set<InterfaceRealization, BehavioredClassifier, InterfaceRealizationPolicy> m_interfaceRealizations = Set<InterfaceRealization, BehavioredClassifier, InterfaceRealizationPolicy>(this);
            Singleton<Behavior, BehavioredClassifier>& getClassifierBehaviorSingleton();
            BehavioredClassifier(std::size_t elementType, AbstractManager& manager);
        public:
            IndexableSet<Behavior, BehavioredClassifier>& getOwnedBehaviors();
            BehaviorPtr getClassifierBehavior() const;
            void setClassifierBehavior(Behavior& behavior);
            void setClassifierBehavior(BehaviorPtr behavior);
            void setClassifierBehavior(ID id);
            Set<InterfaceRealization, BehavioredClassifier, InterfaceRealizationPolicy>& getInterfaceRealizations();
            typedef TypeInfo<std::tuple<Classifier>, BehavioredClassifier> Info;
    };

    template <>
    struct ElementInfo<BehavioredClassifier> : public DefaultInfo {
        static const bool abstract = true;
        inline static const std::string name {"BehavioredClassifier"};
        static SetList sets(BehavioredClassifier& el) {
            return SetList {
                makeSetPair("ownedBehaviors", el.m_ownedBehaviors),
                makeSetPair("classifierBehavior", el.m_classifierBehavior),
                makeSetPair("interfaceRealizations", el.m_interfaceRealizations)
            };
        }
    };
}
