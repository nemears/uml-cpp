#ifndef TEMPLATE_PARAMETER_H
#define TEMPLATE_PARAMETER_H

#include "element.h"
#include "parameterableElement.h"

namespace UML {

    class TemplateSignature;

    class TemplateParameter : public Element {
        private:
            ID m_signatureID;
            TemplateSignature* m_signaturePtr;
            ID m_ownedParameteredElementID;
            ParameterableElement* m_ownedParameteredElementPtr;
        public:
            TemplateParameter();
            TemplateParameter(const TemplateParameter& el);
            virtual ~TemplateParameter();
            TemplateSignature* getSignature();
            void setSignature(TemplateSignature* signature);
            ParameterableElement* getOwnedParameterableElement();
            void setOwnedParameterableElement(ParameterableElement* el);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_PARAMETER;
            };
    };
}

#endif