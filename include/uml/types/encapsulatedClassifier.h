#pragma once

#include "structuredClassifier.h"

namespace UML {

    class Port;

    class EncapsulatedClassifier : public StructuredClassifier {
        protected:
            struct OwnedAttributePolicy {
                void elementAdded(Property& el, EncapsulatedClassifier& me);
                void elementRemoved(Property& el, EncapsulatedClassifier& me);
            };
            ReadOnlySet<Port, EncapsulatedClassifier> m_ownedPorts = ReadOnlySet<Port, EncapsulatedClassifier>(this);
            Set<Property, EncapsulatedClassifier, OwnedAttributePolicy> m_encapsulatedClassifierOwnedAttributes = Set<Property, EncapsulatedClassifier, OwnedAttributePolicy>(this);
            EncapsulatedClassifier();
        public:
            virtual ~EncapsulatedClassifier();
            ReadOnlySet<Port, EncapsulatedClassifier>& getOwnedPorts();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ENCAPSULATED_CLASSIFIER;
            }
    };
}
