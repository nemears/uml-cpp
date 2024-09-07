#pragma once

#include "feature.h"
#include "uml/set/indexableSet.h"
#include "uml/types/element.h"

namespace UML {

    class Behavior;
    class Association;
    class ConnectorEnd;

    typedef UmlPtr<Association> AssociationPtr;

    class Connector : public Feature {

        friend class StructuredClassifier;
        friend struct ElementInfo<Connector>;

        protected:
            class TypePolicy {
                protected:
                void elementAdded(Association& el, Connector& me);
                void elementRemoved(Association& el, Connector& me);
            };
            class EndPolicy : public IndexablePolicy {
                protected:
                void elementAdded(ConnectorEnd& end, Connector& me);
                void elementRemoved(ConnectorEnd& end, Connector& me);
            };
            Singleton<Association, Connector, TypePolicy> m_type = Singleton<Association, Connector, TypePolicy>(this);
            IndexableSet<Behavior, Connector> m_contracts = IndexableSet<Behavior, Connector>(this);
            IndexableOrderedSet<ConnectorEnd, Connector, EndPolicy> m_ends = IndexableOrderedSet<ConnectorEnd, Connector, EndPolicy>(this);
            Singleton<Association, Connector, TypePolicy>& getTypeSingleton();
            Connector(std::size_t elementType, AbstractManager& manager);
        public:
            AssociationPtr getType() const;
            void setType(AssociationPtr type);
            void setType(Association& type);
            void setType(ID id);
            IndexableSet<Behavior, Connector>& getContracts();
            IndexableOrderedSet<ConnectorEnd, Connector, EndPolicy>& getEnds();
            typedef TypeInfo<std::tuple<Feature>, Connector> Info;
    };

    template <>
    struct ElementInfo<Connector> {
        static const bool abstract = false;
        inline static const std::string name {"Connector"};
        static SetList sets(Connector& el) {
            return SetList {
                makeSetPair("type", el.m_type),
                makeSetPair("contracts", el.m_contracts),
                makeSetPair("ends", el.m_ends)
            };
        }
    };

    typedef UmlPtr<Connector> ConnectorPtr;
}
