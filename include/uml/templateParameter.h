#ifndef TEMPLATE_PARAMETER_H
#define TEMPLATE_PARAMETER_H

#include "element.h"

namespace UML {

    class TemplateSignature;
    class ParameterableElement;

    class TemplateParameter : public Element {
        private:
            ID m_signatureID;
            TemplateSignature* m_signaturePtr;
            ID m_ownedParameteredElementID;
            ParameterableElement* m_ownedParameteredElementPtr;
            ID m_parameteredElementID;
            ParameterableElement* m_parameteredElementPtr;
            ID m_defaultID;
            ParameterableElement* m_defaultPtr;
            ID m_ownedDefaultID;
            ParameterableElement* m_ownedDefaultPtr;
        public:
            TemplateParameter();
            TemplateParameter(const TemplateParameter& el);
            virtual ~TemplateParameter();
            TemplateSignature* getSignature();
            void setSignature(TemplateSignature* signature);
            ParameterableElement* getOwnedParameteredElement();
            void setOwnedParameteredElement(ParameterableElement* el);
            ParameterableElement* getParameteredElement();
            void setParameteredElement(ParameterableElement* el);
            ParameterableElement* getDefault();
            void setDefault(ParameterableElement* el);
            ParameterableElement* getOwnedDefault();
            void setOwnedDefault(ParameterableElement* el);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_PARAMETER;
            };
    };
}

#endif