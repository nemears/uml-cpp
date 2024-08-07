#pragma once

#include "classifier.h"
#include "connector.h"
#include "uml/types/property.h"

namespace UML {

    class ConnectableElement;
    class Connector;

    class StructuredClassifier : virtual public Classifier {

        friend class Property;

        protected:
            struct PartPolicy {
                void elementAdded(Property& prop, StructuredClassifier& me);
                void elementRemoved(Property& prop, StructuredClassifier& me);
            };
            class RemovePartPolicy {
                public:
                    static void apply(Property& prop, StructuredClassifier& me);
            };
            ReadOnlySet<ConnectableElement, StructuredClassifier> m_roles = ReadOnlySet<ConnectableElement, StructuredClassifier>(this);
            Set<Property, StructuredClassifier, PartPolicy> m_ownedAttributes = Set<Property, StructuredClassifier, PartPolicy>(this);
            ReadOnlySet<Property, StructuredClassifier> m_parts = ReadOnlySet<Property, StructuredClassifier>(this);
            Set<Connector, StructuredClassifier> m_ownedConnectors = Set<Connector, StructuredClassifier>(this);
            void restoreReferences() override;
            StructuredClassifier();
        public:
            virtual ~StructuredClassifier();
            Set<Property, StructuredClassifier, PartPolicy>& getOwnedAttributes();
            ReadOnlySet<ConnectableElement, StructuredClassifier>& getRoles();
            ReadOnlySet<Property, StructuredClassifier>& getParts();
            Set<Connector, StructuredClassifier>& getOwnedConnectors();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::STRUCTURED_CLASSIFIER;
            };
    };
}
