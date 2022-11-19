#ifndef _UML_INTERFACE_REALIZATION_H_
#define _UML_INTERFACE_REALIZATION_H_

#include "realization.h"
#include "interface.h"

namespace UML {

    typedef UmlPtr<Interface> InterfacePtr;
    typedef UmlPtr<BehavioredClassifier> BehavioredClassifierPtr;

    class InterfaceRealization : public Realization {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        friend class BehavioredClassifier;

        protected:
            class RemoveContractPolicy {
                public:
                    void apply(Interface& el, InterfaceRealization& me);
            };
            class SetContractPolicy {
                public:
                    void apply(Interface& el, InterfaceRealization& me);
            };
            CustomSingleton<Interface, InterfaceRealization, SetContractPolicy, RemoveContractPolicy> m_contract = CustomSingleton<Interface, InterfaceRealization, SetContractPolicy, RemoveContractPolicy>(this);
            CustomSingleton<BehavioredClassifier, InterfaceRealization> m_implementingClassifier = CustomSingleton<BehavioredClassifier, InterfaceRealization>(this);
            TypedSet<Interface, InterfaceRealization>& getContractSingleton();
            TypedSet<BehavioredClassifier, InterfaceRealization>& getImplementingClassifierSingleton();
            void init();
            InterfaceRealization();
        public:
            virtual ~InterfaceRealization();
            InterfacePtr getContract() const;
            void setContract(Interface* contract);
            void setContract(Interface& contract);
            void setContract(ID id);
            BehavioredClassifierPtr getImplementingClassifier() const;
            void setImplementingClassifier(BehavioredClassifier* implementingClassifier);
            void setImplementingClassifier(BehavioredClassifier& implementingClassifier);
            void setImplementingClassifier(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INTERFACE_REALIZATION;
            };
    };
}

#endif