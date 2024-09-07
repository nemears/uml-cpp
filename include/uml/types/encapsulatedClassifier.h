#pragma once

#include "structuredClassifier.h"
#include "uml/types/element.h"

namespace UML {

    class Port;

    class EncapsulatedClassifier : public StructuredClassifier {

        friend struct ElementInfo<EncapsulatedClassifier>;

        protected:
            struct OwnedAttributePolicy {
                void elementAdded(Property& el, EncapsulatedClassifier& me);
                void elementRemoved(Property& el, EncapsulatedClassifier& me);
            };
            ReadOnlyIndexableSet<Port, EncapsulatedClassifier> m_ownedPorts = ReadOnlyIndexableSet<Port, EncapsulatedClassifier>(this);
            IndexableSet<Property, EncapsulatedClassifier, OwnedAttributePolicy> m_encapsulatedClassifierOwnedAttributes = IndexableSet<Property, EncapsulatedClassifier, OwnedAttributePolicy>(this);
            EncapsulatedClassifier(std::size_t elementType, AbstractManager& manager);
        public:
            ReadOnlyIndexableSet<Port, EncapsulatedClassifier>& getOwnedPorts();
            typedef TypeInfo<std::tuple<StructuredClassifier>, EncapsulatedClassifier> Info;
    };

    template <>
    struct ElementInfo<EncapsulatedClassifier> {
        static const bool abstract = true;
        inline static const std::string name {"EncapsulatedClassifier"};
        static SetList sets(EncapsulatedClassifier& el) {
            return SetList {
                makeSetPair("ownedPorts", el.getOwnedPorts()),
                makeSetPair("ownedAttributes", el.m_ownedAttributes)
            };
        }
    };
}
