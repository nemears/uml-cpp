#ifndef _UML_CONNECTABLE_ELEMENT_H_
#define _UML_CONNECTABLE_ELEMENT_H_

#include "typedElement.h"
#include "parameterableElement.h"

namespace UML {
    class ConnectableElement : virtual public TypedElement, virtual public ParameterableElement {
        protected:
            Set<ConnectorEnd, ConnectableElement> m_ends = Set<ConnectorEnd, ConnectableElement>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
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