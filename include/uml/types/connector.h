#pragma once

#include "feature.h"
#include "uml/set/orderedSet.h"

namespace UML {

    class Behavior;
    class Association;
    class ConnectorEnd;

    typedef UmlPtr<Association> AssociationPtr;

    class Connector : public Feature {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class StructuredClassifier;

        protected:
            struct TypePolicy {
                void elementAdded(Association& el, Connector& me);
                void elementRemoved(Association& el, Connector& me);
            };
            struct EndPolicy {
                void elementAdded(ConnectorEnd& end, Connector& me);
                void elementRemoved(ConnectorEnd& end, Connector& me);
            };
            Singleton<Association, Connector, TypePolicy> m_type = Singleton<Association, Connector, TypePolicy>(this);
            Set<Behavior, Connector> m_contracts = Set<Behavior, Connector>(this);
            OrderedSet<ConnectorEnd, Connector, EndPolicy> m_ends = OrderedSet<ConnectorEnd, Connector, EndPolicy>(this);
            Singleton<Association, Connector, TypePolicy>& getTypeSingleton();
            void referenceErased(ID id) override;
            void restoreReferences() override;
            Connector();
        public:
            virtual ~Connector();
            AssociationPtr getType() const;
            void setType(Association* type);
            void setType(Association& type);
            void setType(ID id);
            Set<Behavior, Connector>& getContracts();
            OrderedSet<ConnectorEnd, Connector, EndPolicy>& getEnds();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CONNECTOR;
            };
    };

    typedef UmlPtr<Connector> ConnectorPtr;
}
