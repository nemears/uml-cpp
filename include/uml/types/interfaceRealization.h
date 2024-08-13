#pragma once

#include "realization.h"

namespace UML {

    class Interface;
    class BehavioredClassifier;
    typedef UmlPtr<Interface> InterfacePtr;
    typedef UmlPtr<BehavioredClassifier> BehavioredClassifierPtr;

    class InterfaceRealization : public Realization {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class BehavioredClassifier;

        protected:
            struct ContractPolicy {
                void elementAdded(Interface& el, InterfaceRealization& me);
                void elementRemoved(Interface& el, InterfaceRealization& me);
            };
            Singleton<Interface, InterfaceRealization, ContractPolicy> m_contract = Singleton<Interface, InterfaceRealization, ContractPolicy>(this);
            Singleton<BehavioredClassifier, InterfaceRealization> m_implementingClassifier = Singleton<BehavioredClassifier, InterfaceRealization>(this);
            Singleton<Interface, InterfaceRealization, ContractPolicy>& getContractSingleton();
            Singleton<BehavioredClassifier, InterfaceRealization>& getImplementingClassifierSingleton();
            InterfaceRealization();
        public:
            virtual ~InterfaceRealization();
            InterfacePtr getContract() const;
            void setContract(InterfacePtr contract);
            void setContract(Interface& contract);
            void setContract(ID id);
            BehavioredClassifierPtr getImplementingClassifier() const;
            void setImplementingClassifier(BehavioredClassifierPtr implementingClassifier);
            void setImplementingClassifier(BehavioredClassifier& implementingClassifier);
            void setImplementingClassifier(ID id);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INTERFACE_REALIZATION;
            };
    };
}
