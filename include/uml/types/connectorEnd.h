#pragma once

#include "multiplicityElement.h"
#include "uml/managers/typeInfo.h"
#include "uml/types/element.h"

namespace UML {

    class ConnectableElement;
    class Connector;
    class Property;
    
    typedef UmlPtr<ConnectableElement> ConnectableElementPtr;
    typedef UmlPtr<Property> PropertyPtr;

    class ConnectorEnd : public MultiplicityElement {

        friend class Connector;
        friend class ConnectableElement;
        friend struct ElementInfo<ConnectorEnd>;

        protected:
            Singleton<ConnectableElement, ConnectorEnd> m_role = Singleton<ConnectableElement, ConnectorEnd>(this);
            Singleton<Property, ConnectorEnd> m_definingEnd = Singleton<Property, ConnectorEnd>(this);
            Singleton<ConnectableElement, ConnectorEnd>& getRoleSingleton();
            Singleton<Property, ConnectorEnd>& getDefiningEndSingleton();
        public:
            ConnectorEnd(std::size_t elementType, AbstractManager& manager);
            ConnectableElementPtr getRole() const;
            void setRole(ConnectableElementPtr role);
            void setRole(ConnectableElement& role);
            void setRole(ID id);
            PropertyPtr getDefiningEnd() const;
            typedef TypeInfo<std::tuple<MultiplicityElement>, ConnectorEnd> Info;
    };

    template <>
    struct ElementInfo<ConnectorEnd> {
        static const bool abstract = false;
        inline static const std::string name {"ConnectorEnd"};
        static SetList sets(ConnectorEnd& el) {
            return SetList {
                makeSetPair("role", el.m_role),
                makeSetPair("definingEnd", el.m_definingEnd)
            };
        }
    };
}
