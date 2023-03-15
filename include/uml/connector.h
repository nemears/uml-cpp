#ifndef _UML_CONNECTOR_H
#define _UML_CONNECTOR_H

#include "feature.h"
#include "connectorEnd.h"
#include "set/orderedSet.h"

namespace UML {

    class Behavior;

    typedef UmlPtr<Association> AssociationPtr;

    class Connector : public Feature {

        template <typename AccessPolicy, typename SerializationPolicy> friend class Manager;
        friend class StructuredClassifier;

        protected:
            class SetTypePolicy {
                public:
                    void apply(Association& el, Connector& me);
            };
            class RemoveTypePolicy {
                public:
                    void apply(Association& el, Connector& me);
            };
            class AddEndPolicy {
                public:
                    void apply(ConnectorEnd& end, Connector& me);
            };
            class RemoveEndPolicy {
                public:
                    void apply(ConnectorEnd& end, Connector& me);
            };
            CustomSingleton<Association, Connector, SetTypePolicy, RemoveTypePolicy> m_type = CustomSingleton<Association, Connector, SetTypePolicy, RemoveTypePolicy>(this);
            CustomSet<Behavior, Connector> m_contracts = CustomSet<Behavior, Connector>(this);
            CustomOrderedSet<ConnectorEnd, Connector, AddEndPolicy, RemoveEndPolicy> m_ends = CustomOrderedSet<ConnectorEnd, Connector, AddEndPolicy, RemoveEndPolicy>(this);
            CustomSingleton<StructuredClassifier, Connector> m_structuredClassifier = CustomSingleton<StructuredClassifier, Connector>(this);
            TypedSet<Association, Connector>& getTypeSingleton();
            TypedSet<StructuredClassifier, Connector>& getStructuredClassifierSingleton();
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
            Connector();
        public:
            virtual ~Connector();
            AssociationPtr getType() const;
            void setType(Association* type);
            void setType(Association& type);
            void setType(ID id);
            Set<Behavior, Connector>& getContracts();
            OrderedSet<ConnectorEnd, Connector>& getEnds();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CONNECTOR;
            };
    };

    typedef UmlPtr<Connector> ConnectorPtr;
}

#endif