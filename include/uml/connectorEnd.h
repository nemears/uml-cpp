#ifndef _UML_CONNECTOR_END_H_
#define _UML_CONNECTOR_END_H_

#include "multiplicityElement.h"

namespace UML {

    class ConnectableElement;
    class Connector;
    
    typedef UmlPtr<ConnectableElement> ConnectableElementPtr;
    typedef UmlPtr<Property> PropertyPtr;

    class ConnectorEnd : public MultiplicityElement {

        friend class UmlManager;
        friend class Connector;

        protected:
            Singleton<ConnectableElement, ConnectorEnd> m_role = Singleton<ConnectableElement, ConnectorEnd>(this);
            Singleton<Property, ConnectorEnd> m_definingEnd = Singleton<Property, ConnectorEnd>(this);
            Set<ConnectableElement, ConnectorEnd>& getRoleSingleton();
            Set<Property, ConnectorEnd>& getDefiningEndSingleton();
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
            void referencingReleased(ID id) override;
            void referenceErased(ID id) override;
            void init();
            ConnectorEnd();
        public:
            virtual ~ConnectorEnd();
            ConnectableElementPtr getRole() const;
            void setRole(ConnectableElement* role);
            void setRole(ConnectableElement& role);
            void setRole(ID id);
            PropertyPtr getDefiningEnd() const;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CONNECTOR_END;
            }
    };
}

#endif