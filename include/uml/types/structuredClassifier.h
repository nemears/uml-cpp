#pragma once

#include "classifier.h"
#include "connector.h"
#include "uml/set/indexableSet.h"
#include "uml/types/element.h"
#include "uml/types/property.h"

namespace UML {

    class ConnectableElement;
    class Connector;

    class StructuredClassifier : virtual public Classifier {

        friend class Property;
        friend struct ElementInfo<StructuredClassifier>;

        protected:
            struct PartPolicy : IndexablePolicy {
                void elementAdded(Property& prop, StructuredClassifier& me);
                void elementRemoved(Property& prop, StructuredClassifier& me);
            };
            ReadOnlyIndexableSet<ConnectableElement, StructuredClassifier> m_roles = ReadOnlyIndexableSet<ConnectableElement, StructuredClassifier>(this);
            IndexableSet<Property, StructuredClassifier, PartPolicy> m_ownedAttributes = IndexableSet<Property, StructuredClassifier, PartPolicy>(this);
            ReadOnlyIndexableSet<Property, StructuredClassifier> m_parts = ReadOnlyIndexableSet<Property, StructuredClassifier>(this);
            IndexableSet<Connector, StructuredClassifier> m_ownedConnectors = IndexableSet<Connector, StructuredClassifier>(this);
            StructuredClassifier(std::size_t elementType, AbstractManager& manager);
        public:
            IndexableSet<Property, StructuredClassifier, PartPolicy>& getOwnedAttributes();
            ReadOnlyIndexableSet<ConnectableElement, StructuredClassifier>& getRoles();
            ReadOnlyIndexableSet<Property, StructuredClassifier>& getParts();
            IndexableSet<Connector, StructuredClassifier>& getOwnedConnectors();
            typedef TypeInfo<std::tuple<Classifier>, StructuredClassifier> Info;
    };

    template <>
    struct ElementInfo<StructuredClassifier> {
        static const bool abstract = true;
        inline static const std::string name {"StructuredClassifier"};
        static SetList sets(StructuredClassifier& el) {
            return SetList {
                makeSetPair("roles", el.m_roles),
                makeSetPair("ownedAttributes", el.m_ownedAttributes),
                makeSetPair("parts", el.m_parts),
                makeSetPair("ownedConnectors", el.m_ownedConnectors)
            };
        }
    };
}
