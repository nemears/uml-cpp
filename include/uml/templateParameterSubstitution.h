#ifndef TEMPLATE_PARAMETER_SUBSTITUTION_H
#define TEMPLATE_PARAMETER_SUBSTITUTION_H

#include "element.h"
#include "templateParameter.h"

namespace UML {

    class TemplateBinding;

    class TemplateParameterSubstitution : public Element {
        private:
            ID m_formalID;
            TemplateParameter* m_formalPtr;
            ID m_templateBindingID;
            TemplateBinding* m_templateBindingPtr;
        public:
            TemplateParameterSubstitution();
            TemplateParameterSubstitution(const TemplateParameterSubstitution& sub);
            ~TemplateParameterSubstitution();
            TemplateParameter* getFormal();
            void setFormal(TemplateParameter* formal);
            TemplateBinding* getTemplateBinding();
            void setTemplateBinding(TemplateBinding* binding);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
            };
    };
}

#endif