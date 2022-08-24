#ifndef _UML_PIN_H_
#define _UML_PIN_H_

#include "objectNode.h"
#include "multiplicityElement.h"

namespace UML {
    class Pin : public ObjectNode , public MultiplicityElement {
        protected:
            bool m_isControl = false;
            Pin();
        public:
            virtual ~Pin();
            bool isControl() const;
            void setIsControl(bool val);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PIN;
            };
    };
}

#endif