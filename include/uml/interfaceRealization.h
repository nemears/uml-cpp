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
            Singleton<Interface, InterfaceRealization> m_contract = Singleton<Interface, InterfaceRealization>(this);
            Singleton<BehavioredClassifier, InterfaceRealization> m_implementingClassifier = Singleton<BehavioredClassifier, InterfaceRealization>(this);
            class RemoveContractFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveContractFunctor(Element* el) : SetFunctor(el) {};
            };
            class SetContractFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    SetContractFunctor(Element* el) : SetFunctor(el) {};
            };
            Set<Interface, InterfaceRealization>& getContractSingleton();
            Set<BehavioredClassifier, InterfaceRealization>& getImplementingClassifierSingleton();
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