#ifndef _UML_CONNECTOR_END_H
#define _UML_CONNECTOR_END_H

#include "multiplicityElement.h"

namespace UML {

    class ConnectableElement;

    class ConnectorEnd : public MultiplicityElement {

        friend class UmlManager;

        protected:
            Singleton<ConnectableElement, ConnectorEnd> m_role = Singleton<ConnectableElement, ConnectorEnd>(this);
            Singleton<Property, ConnectorEnd> m_definingEnd = Singleton<Property, ConnectorEnd>(this);
            Set<ConnectableElement, ConnectorEnd>& getRoleSingleton();
            Set<Property, ConnectorEnd>& getDefiningEndSingleton();
            void init();
            void copy(const ConnectorEnd& rhs);
            ConnectorEnd();
        public:
            ConnectorEnd(const ConnectorEnd& rhs);
            virtual ~ConnectorEnd();
            ConnectableElement* getRole();
            ConnectableElement& getRoleRef();
            ID getRoleID() const;
            bool hasRole() const;
            void setRole(ConnectableElement* role);
            void setRole(ConnectableElement& role);
            void setRole(ID id);
            Property* getDefiningEnd();
            Property& getDefiningEndRef();
            ID getDefiningEndID() const;
            bool hasDefiningEnd() const;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CONNECTOR_END;
            }
    };
}

#endif