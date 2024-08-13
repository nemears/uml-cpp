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
            ReadOnlyIndexableSet<Port, EncapsulatedClassifier> m_ownedPorts = ReadOnlyIndexableSet<Port, EncapsulatedClassifier>(this);
            IndexableSet<Property, EncapsulatedClassifier, OwnedAttributePolicy> m_encapsulatedClassifierOwnedAttributes = IndexableSet<Property, EncapsulatedClassifier, OwnedAttributePolicy>(this);
            EncapsulatedClassifier();
        public:
            virtual ~EncapsulatedClassifier();
            ReadOnlyIndexableSet<Port, EncapsulatedClassifier>& getOwnedPorts();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ENCAPSULATED_CLASSIFIER;
            }
    };
}
