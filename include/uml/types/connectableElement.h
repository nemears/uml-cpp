#pragma once

#include "typedElement.h"
#include "parameterableElement.h"
#include "uml/set/set.h"

namespace UML {

    class ConnectorEnd;

    class ConnectableElement : virtual public TypedElement, virtual public ParameterableElement {

        protected:
            ReadOnlySet<ConnectorEnd, ConnectableElement> m_ends = ReadOnlySet<ConnectorEnd, ConnectableElement>(this);
            void referenceErased(ID id) override;
            ConnectableElement();
        public:
            virtual ~ConnectableElement();
            ReadOnlySet<ConnectorEnd, ConnectableElement>& getEnds();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CONNECTABLE_ELEMENT;
            };
    };
}
