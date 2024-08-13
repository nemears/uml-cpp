#pragma once

#include "multiplicityElement.h"

namespace UML {

    class ConnectableElement;
    class Connector;
    class Property;
    
    typedef UmlPtr<ConnectableElement> ConnectableElementPtr;
    typedef UmlPtr<Property> PropertyPtr;

    class ConnectorEnd : public MultiplicityElement {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Connector;
        friend class ConnectableElement;

        protected:
            Singleton<ConnectableElement, ConnectorEnd> m_role = Singleton<ConnectableElement, ConnectorEnd>(this);
            Singleton<Property, ConnectorEnd> m_definingEnd = Singleton<Property, ConnectorEnd>(this);
            Singleton<ConnectableElement, ConnectorEnd>& getRoleSingleton();
            Singleton<Property, ConnectorEnd>& getDefiningEndSingleton();
            void referenceErased(ID id) override;
            ConnectorEnd();
        public:
            virtual ~ConnectorEnd();
            ConnectableElementPtr getRole() const;
            void setRole(ConnectableElementPtr role);
            void setRole(ConnectableElement& role);
            void setRole(ID id);
            PropertyPtr getDefiningEnd() const;
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CONNECTOR_END;
            }
    };
}
