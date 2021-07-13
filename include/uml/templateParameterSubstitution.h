#ifndef TEMPLATE_PARAMETER_SUBSTITUTION_H
#define TEMPLATE_PARAMETER_SUBSTITUTION_H

#include "element.h"

namespace UML {

    class TemplateBinding;
    class TemplateParameter;
    class ParameterableElement;

    class TemplateParameterSubstitution : public Element {
        private:
            ID m_formalID;
            TemplateParameter* m_formalPtr;
            ID m_templateBindingID;
            TemplateBinding* m_templateBindingPtr;
            ID m_actualID;
            ParameterableElement* m_actualPtr;
            ID m_ownedActualID;
            ParameterableElement* m_ownedActualPtr;
        public:
            TemplateParameterSubstitution();
            TemplateParameterSubstitution(const TemplateParameterSubstitution& sub);
            ~TemplateParameterSubstitution();
            TemplateParameter* getFormal();
            void setFormal(TemplateParameter* formal);
            TemplateBinding* getTemplateBinding();
            void setTemplateBinding(TemplateBinding* binding);
            ParameterableElement* getActual();
            void setActual(ParameterableElement* actual);
            ParameterableElement* getOwnedActual();
            void setOwnedActual(ParameterableElement* actual);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
            };
    };
}

#endif