#ifndef _UML_CONNECTABLE_ELEMENT_H_
#define _UML_CONNECTABLE_ELEMENT_H_

#include "typedElement.h"
#include "parameterableElement.h"

namespace UML {

    class ConnectorEnd;

    class ConnectableElement : virtual public TypedElement, virtual public ParameterableElement {
        protected:
            CustomSet<ConnectorEnd, ConnectableElement> m_ends = CustomSet<ConnectorEnd, ConnectableElement>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
            void init();
            ConnectableElement();
        public:
            virtual ~ConnectableElement();
            Set<ConnectorEnd, ConnectableElement>& getEnds();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CONNECTABLE_ELEMENT;
            };
    };
}

#endif