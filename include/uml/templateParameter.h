#ifndef TEMPLATE_PARAMETER_H
#define TEMPLATE_PARAMETER_H

#include "element.h"

namespace UML {
    class TemplateParameter : public Element {
        private:
        public:
            TemplateParameter();
            virtual ~TemplateParameter();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_PARAMETER;
            };
    };
}

#endif