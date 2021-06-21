#ifndef TEMPLATE_PARAMETER_SUBSTITUTION_H
#define TEMPLATE_PARAMETER_SUBSTITUTION_H

#include "element.h"

namespace UML {
    class TemplateParameterSubstitution : public Element {
        private:
        public:
            TemplateParameterSubstitution();
            TemplateParameterSubstitution(const TemplateParameterSubstitution& sub);
            ~TemplateParameterSubstitution();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
            };
    };
}

#endif