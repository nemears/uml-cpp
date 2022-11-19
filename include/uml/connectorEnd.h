#ifndef _UML_CONNECTOR_END_H_
#define _UML_CONNECTOR_END_H_

#include "multiplicityElement.h"

namespace UML {

    class ConnectableElement;
    class Connector;
    
    typedef UmlPtr<ConnectableElement> ConnectableElementPtr;
    typedef UmlPtr<Property> PropertyPtr;

    class ConnectorEnd : public MultiplicityElement {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        friend class Connector;

        protected:
            CustomSingleton<ConnectableElement, ConnectorEnd> m_role = CustomSingleton<ConnectableElement, ConnectorEnd>(this);
            CustomSingleton<Property, ConnectorEnd> m_definingEnd = CustomSingleton<Property, ConnectorEnd>(this);
            TypedSet<ConnectableElement, ConnectorEnd>& getRoleSingleton();
            TypedSet<Property, ConnectorEnd>& getDefiningEndSingleton();
            void referenceReindexed(ID oldID, ID newID) override;
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