#pragma once

#include "classifier.h"
#include "connector.h"
#include "uml/set/indexableSet.h"
#include "uml/types/property.h"

namespace UML {

    class ConnectableElement;
    class Connector;

    class StructuredClassifier : virtual public Classifier {

        friend class Property;

        protected:
            struct PartPolicy : IndexablePolicy {
                void elementAdded(Property& prop, StructuredClassifier& me);
                void elementRemoved(Property& prop, StructuredClassifier& me);
            };
            ReadOnlyIndexableSet<ConnectableElement, StructuredClassifier> m_roles = ReadOnlyIndexableSet<ConnectableElement, StructuredClassifier>(this);
            IndexableSet<Property, StructuredClassifier, PartPolicy> m_ownedAttributes = IndexableSet<Property, StructuredClassifier, PartPolicy>(this);
            ReadOnlyIndexableSet<Property, StructuredClassifier> m_parts = ReadOnlyIndexableSet<Property, StructuredClassifier>(this);
            IndexableSet<Connector, StructuredClassifier> m_ownedConnectors = IndexableSet<Connector, StructuredClassifier>(this);
            void restoreReferences() override;
            StructuredClassifier();
        public:
            virtual ~StructuredClassifier();
            IndexableSet<Property, StructuredClassifier, PartPolicy>& getOwnedAttributes();
            ReadOnlyIndexableSet<ConnectableElement, StructuredClassifier>& getRoles();
            ReadOnlyIndexableSet<Property, StructuredClassifier>& getParts();
            IndexableSet<Connector, StructuredClassifier>& getOwnedConnectors();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::STRUCTURED_CLASSIFIER;
            };
    };
}
