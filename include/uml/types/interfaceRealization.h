#pragma once

#include "realization.h"
#include "uml/types/element.h"

namespace UML {

    class Interface;
    class BehavioredClassifier;
    typedef UmlPtr<Interface> InterfacePtr;
    typedef UmlPtr<BehavioredClassifier> BehavioredClassifierPtr;

    class InterfaceRealization : public Realization {

        friend class BehavioredClassifier;
        friend struct ElementInfo<InterfaceRealization>;

        protected:
            struct ContractPolicy {
                void elementAdded(Interface& el, InterfaceRealization& me);
                void elementRemoved(Interface& el, InterfaceRealization& me);
            };
            Singleton<Interface, InterfaceRealization, ContractPolicy> m_contract = Singleton<Interface, InterfaceRealization, ContractPolicy>(this);
            Singleton<BehavioredClassifier, InterfaceRealization> m_implementingClassifier = Singleton<BehavioredClassifier, InterfaceRealization>(this);
            Singleton<Interface, InterfaceRealization, ContractPolicy>& getContractSingleton();
            Singleton<BehavioredClassifier, InterfaceRealization>& getImplementingClassifierSingleton();
            InterfaceRealization(std::size_t elementType, AbstractManager& manager);
        public:
            InterfacePtr getContract() const;
            void setContract(InterfacePtr contract);
            void setContract(Interface& contract);
            void setContract(ID id);
            BehavioredClassifierPtr getImplementingClassifier() const;
            void setImplementingClassifier(BehavioredClassifierPtr implementingClassifier);
            void setImplementingClassifier(BehavioredClassifier& implementingClassifier);
            void setImplementingClassifier(ID id);
            typedef TypeInfo<std::tuple<Realization>, InterfaceRealization> Info;
    };

    template <>
    struct ElementInfo<InterfaceRealization> {
        static const bool abstract = false;
        inline static const std::string name {"InterfaceRealization"};
        static SetList sets(InterfaceRealization& el) {
            return SetList {
                makeSetPair("contract", el.m_contract),
                makeSetPair("implementingClassifier", el.m_implementingClassifier),
            };
        }
    };
}
