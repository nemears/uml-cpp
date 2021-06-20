#ifndef TEMPLATE_PARAMETER_H
#define TEMPLATE_PARAMETER_H

#include "element.h"

namespace UML {

    class TemplateSignature;

    class TemplateParameter : public Element {
        private:
            ID m_signatureID;
            TemplateSignature* m_signaturePtr;
        public:
            TemplateParameter();
            TemplateParameter(const TemplateParameter& el);
            virtual ~TemplateParameter();
            TemplateSignature* getSignature();
            void setSignature(TemplateSignature* signature);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_PARAMETER;
            };
    };
}

#endif