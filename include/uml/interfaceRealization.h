#ifndef _UML_INTERFACE_REALIZATION_H_
#define _UML_INTERFACE_REALIZATION_H_

#include "realization.h"

namespace UML {
    class InterfaceRealization : public Realization {

        friend class UmlManager;
        friend class BehavioredClassifier;

        protected:
            Singleton<Interface, InterfaceRealization> m_contract = Singleton<Interface, InterfaceRealization>(this);
            Singleton<BehavioredClassifier, InterfaceRealization> m_implementingClassifier = Singleton<BehavioredClassifier, InterfaceRealization>(this);
            Set<Interface, InterfaceRealization>& getContractSingleton();
            Set<BehavioredClassifier, InterfaceRealization>& getImplementingClassifierSingleton();
            void init();
            void copy(const InterfaceRealization& rhs);
            InterfaceRealization();
        public:
            virtual ~InterfaceRealization();
            InterfaceRealization(const InterfaceRealization& rhs);
            Interface* getContract();
            Interface& getContractRef();
            bool hasContract() const;
            ID getContractID() const;
            void setContract(Interface* contract);
            void setContract(Interface& contract);
            void setContract(ID id);
            BehavioredClassifier* getImplementingClassifier();
            BehavioredClassifier& getImplementingClassifierRef();
            bool hasImplementingClassifier() const;
            ID getImplementingClassifierID() const;
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