#ifndef CONNECTABLE_ELEMENT_H
#define CONNECTABLE_ELEMENT_H

#include "typedElement.h"
#include "parameterableElement.h"

namespace UML {
    class ConnectableElement : virtual public TypedElement, virtual public ParameterableElement {
        protected:
            Set<ConnectorEnd, ConnectableElement> m_ends = Set<ConnectorEnd, ConnectableElement>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            void init();
            void copy(const ConnectableElement& rhs);
            ConnectableElement();
        public:
            ConnectableElement(const ConnectableElement& rhs);
            virtual ~ConnectableElement();
            Set<ConnectorEnd, ConnectableElement>& getEnds();
            bool isSubClassOf(ElementType eType) const override;
            ConnectableElement& operator=(ConnectableElement&&) {
                return *this;
            };
            static ElementType elementType() {
                return ElementType::CONNECTABLE_ELEMENT;
            };
    };
}

#endif