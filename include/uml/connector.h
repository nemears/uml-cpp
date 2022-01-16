#ifndef _UML_CONNECTOR_H
#define _UML_CONNECTOR_H

#include "feature.h"
#include "orderedSet.h"

namespace UML {
    class Connector : public Feature {
        protected:
            Singleton<Association, Connector> m_type = Singleton<Association, Connector>(this);
            Set<Behavior, Connector> m_contracts = Set<Behavior, Connector>(this);
            OrderedSet<ConnectorEnd, Connector> m_ends = OrderedSet<ConnectorEnd, Connector>(this);
            Set<Association, Connector>& getTypeSingleton();
            Set<ConnectorEnd, Connector>& getEndsSet();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            void init();
            void copy(const Connector& rhs);
            Connector();
        public:
            Connector(const Connector& rhs);
            virtual ~Connector();
            Association* getType();
            Association& getTypeRef();
            ID getTypeID() const;
            bool hasType() const;
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