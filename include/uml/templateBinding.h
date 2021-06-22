#ifndef TEMPLATE_BINDING_H
#define TEMPLATE_BINDING_H

#include "directedRelationship.h"
#include "templateableElement.h"
#include "templateParameterSubstitution.h"

namespace UML {
    class TemplateBinding : public DirectedRelationship {
        private:
            ID m_boundElementID;
            TemplateableElement* m_boundElementPtr;
            ID m_signatureID;
            TemplateSignature* m_signaturePtr;
            ID m_parameterSubstitutionID;
            TemplateParameterSubstitution* m_parameterSubstitutionPtr;
        public:
            TemplateBinding();
            TemplateBinding(const TemplateBinding& bind);
            ~TemplateBinding();
            TemplateableElement* getBoundElement();
            void setBoundElement(TemplateableElement* el);
            TemplateSignature* getSignature();
            void setSignature(TemplateSignature* signature);
            TemplateParameterSubstitution* getParameterSubstitution();
            void setParameterSubstitution(TemplateParameterSubstitution* sub);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_BINDING;
            };
    };
}

#endif