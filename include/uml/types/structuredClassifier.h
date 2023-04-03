#ifndef _UML_STRUCTURED_CLASSIFIER_H_
#define _UML_STRUCTURED_CLASSIFIER_H_

#include "classifier.h"
#include "connector.h"

namespace UML {

    class ConnectableElement;
    class Connector;

    class StructuredClassifier : virtual public Classifier {

        friend class Property;

        protected:
            class AddPartPolicy {
                public:
                    static void apply(Property& prop, StructuredClassifier& me);
            };
            class RemovePartPolicy {
                public:
                    static void apply(Property& prop, StructuredClassifier& me);
            };
            CustomReadOnlySet<ConnectableElement, StructuredClassifier> m_roles = CustomReadOnlySet<ConnectableElement, StructuredClassifier>(this);
            CustomSet<Property, StructuredClassifier, AddPartPolicy, RemovePartPolicy> m_ownedAttributes = CustomSet<Property, StructuredClassifier, AddPartPolicy, RemovePartPolicy>(this);
            CustomReadOnlySet<Property, StructuredClassifier> m_parts = CustomReadOnlySet<Property, StructuredClassifier>(this);
            CustomSet<Connector, StructuredClassifier> m_ownedConnectors = CustomSet<Connector, StructuredClassifier>(this);
            void restoreReferences() override;
            void restoreReference(Element* el) override;
            StructuredClassifier();
        public:
            virtual ~StructuredClassifier();
            Set<Property, StructuredClassifier>& getOwnedAttributes();
            ReadOnlySet<ConnectableElement, StructuredClassifier>& getRoles();
            ReadOnlySet<Property, StructuredClassifier>& getParts();
            Set<Connector, StructuredClassifier>& getOwnedConnectors();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::STRUCTURED_CLASSIFIER;
            };
    };
}

#endif