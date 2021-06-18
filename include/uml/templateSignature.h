#ifndef TEMPLATE_SIGNATURE_H
#define TEMPLATE_SIGNATURE_H

#include "element.h"

namespace UML {
    class TemplateSignature : public Element {
        public:
            virtual ~TemplateSignature();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_SIGNATURE;
            };
    };
}

#endif