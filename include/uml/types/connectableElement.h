#ifndef _UML_CONNECTABLE_ELEMENT_H_
#define _UML_CONNECTABLE_ELEMENT_H_

#include "typedElement.h"
#include "parameterableElement.h"
#include "uml/set/doNothingPolicy.h"

namespace UML {

    class ConnectorEnd;

    class ConnectableElement : virtual public TypedElement, virtual public ParameterableElement {

        protected:
            ReadOnlySet<ConnectorEnd, DoNothingPolicy, ConnectableElement> m_ends = CustomReadOnlySet<ConnectorEnd, ConnectableElement>(this);
            void referenceErased(ID id) override;
            ConnectableElement();
        public:
            virtual ~ConnectableElement();
            ReadOnlySet<ConnectorEnd, DoNothingPolicy, ConnectableElement>& getEnds();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CONNECTABLE_ELEMENT;
            };
    };
}

#endif
