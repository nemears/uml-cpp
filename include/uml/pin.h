#ifndef PIN_H
#define PIN_H

#include "objectNode.h"
#include "multiplicityElement.h"

namespace UML {
    class Pin : public ObjectNode , public MultiplicityElement {
        protected:
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override {}; // TODO
            void referenceErased(ID id) override {}; // TODO
        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PIN;
            };
    };
}

#endif