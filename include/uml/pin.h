#ifndef PIN_H
#define PIN_H

#include "objectNode.h"
#include "multiplicityElement.h"

namespace UML {
    class Pin : public ObjectNode , public MultiplicityElement {

        friend class UmlManager;

        protected:
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override {}; // TODO
            void referenceErased(ID id) override {}; // TODO
            Pin();
        public:
            Pin(const Pin& rhs);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PIN;
            };
    };
}

#endif