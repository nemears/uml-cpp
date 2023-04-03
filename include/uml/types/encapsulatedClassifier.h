#ifndef _UML_ENCAPSULATED_CLASSIFIER_H_
#define _UML_ENCAPSULATED_CLASSIFIER_H_

#include "structuredClassifier.h"
#include "port.h"

namespace UML {

    class Port;

    class EncapsulatedClassifier : public StructuredClassifier {
        protected:
            class AddOwnedAttributePolicy {
                public:
                    void apply(Property& el, EncapsulatedClassifier& me);
            };
            class RemoveOwnedAttributePolicy {
                public:
                    void apply(Property& el, EncapsulatedClassifier& me);
            };
            CustomReadOnlySet<Port, EncapsulatedClassifier> m_ownedPorts = CustomReadOnlySet<Port, EncapsulatedClassifier>(this);
            CustomSet<Property, EncapsulatedClassifier, AddOwnedAttributePolicy, RemoveOwnedAttributePolicy> m_encapsulatedClassifierOwnedAttributes = CustomSet<Property, EncapsulatedClassifier, AddOwnedAttributePolicy, RemoveOwnedAttributePolicy>(this);
            EncapsulatedClassifier();
        public:
            virtual ~EncapsulatedClassifier();
            ReadOnlySet<Port, EncapsulatedClassifier>& getOwnedPorts();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ENCAPSULATED_CLASSIFIER;
            }
    };
}

#endif