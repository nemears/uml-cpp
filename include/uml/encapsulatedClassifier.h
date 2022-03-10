#ifndef _UML_ENCAPSULATED_CLASSIFIER_H_
#define _UML_ENCAPSULATED_CLASSIFIER_H_

#include "structuredClassifier.h"
#include "port.h"

namespace UML {

    class Port;

    class EncapsulatedClassifier : public StructuredClassifier {
        protected:
            Set<Port, EncapsulatedClassifier> m_ownedPorts = Set<Port, EncapsulatedClassifier>(this);
            class AddOwnedAttributeFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    AddOwnedAttributeFunctor(Element* el) : SetFunctor(el) {};
            };
            class RemoveOwnedAttributeFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveOwnedAttributeFunctor(Element* el) : SetFunctor(el) {};
            };
            void init();
            EncapsulatedClassifier();
        public:
            virtual ~EncapsulatedClassifier();
            Set<Port, EncapsulatedClassifier>& getOwnedPorts();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ENCAPSULATED_CLASSIFIER;
            }
    };
}

#endif