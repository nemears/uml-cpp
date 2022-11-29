#ifndef _UML_CONNECTOR_H
#define _UML_CONNECTOR_H

#include "feature.h"
#include "connectorEnd.h"
#include "set/orderedSet.h"

namespace UML {

    class Behavior;

    typedef UmlPtr<Association> AssociationPtr;

    class Connector : public Feature {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

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
            CustomSingleton<Association, Connector> m_type = CustomSingleton<Association, Connector>(this);
            CustomSet<Behavior, Connector> m_contracts = CustomSet<Behavior, Connector>(this);
            CustomOrderedSet<ConnectorEnd, Connector> m_ends = CustomOrderedSet<ConnectorEnd, Connector>(this);
            TypedSet<Association, Connector>& getTypeSingleton();
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
            void init();
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
}

#endif