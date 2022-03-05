#ifndef _UML_CONNECTOR_H
#define _UML_CONNECTOR_H

#include "feature.h"
#include "connectorEnd.h"
#include "orderedSet.h"

namespace UML {

    typedef UmlPtr<Association> AssociationPtr;

    class Connector : public Feature {

        friend class UmlManager;

        protected:
            Singleton<Association, Connector> m_type = Singleton<Association, Connector>(this);
            Set<Behavior, Connector> m_contracts = Set<Behavior, Connector>(this);
            OrderedSet<ConnectorEnd, Connector> m_ends = OrderedSet<ConnectorEnd, Connector>(this);
            Set<Association, Connector>& getTypeSingleton();
            Set<ConnectorEnd, Connector>& getEndsSet();
            class SetTypeFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    SetTypeFunctor(Element* el) : SetFunctor(el) {};
            };
            class RemoveTypeFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveTypeFunctor(Element* el) : SetFunctor(el) {};
            };
            class AddEndFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    AddEndFunctor(Element* el) : SetFunctor(el) {};
            };
            class RemoveEndFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveEndFunctor(Element* el) : SetFunctor(el) {};
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
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