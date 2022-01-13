#ifndef CONNECTABLE_ELEMENT_H
#define CONNECTABLE_ELEMENT_H

#include "typedElement.h"
#include "parameterableElement.h"

namespace UML {
    class ConnectableElement : virtual public TypedElement, virtual public ParameterableElement {
        protected:
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReference(Element* el) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            ConnectableElement();
        public:
            ConnectableElement(const ConnectableElement& rhs);
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