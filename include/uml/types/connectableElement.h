#ifndef _UML_CONNECTABLE_ELEMENT_H_
#define _UML_CONNECTABLE_ELEMENT_H_

#include "typedElement.h"
#include "parameterableElement.h"
#include "uml/set/readOnlySet.h"

namespace UML {

    class ConnectorEnd;

    class ConnectableElement : virtual public TypedElement, virtual public ParameterableElement {

        protected:
            CustomReadOnlySet<ConnectorEnd, ConnectableElement> m_ends = CustomReadOnlySet<ConnectorEnd, ConnectableElement>(this);
            void referenceErased(ID id) override;
            ConnectableElement();
        public:
            virtual ~ConnectableElement();
            ReadOnlySet<ConnectorEnd, ConnectableElement>& getEnds();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CONNECTABLE_ELEMENT;
            };
    };
}

#endif