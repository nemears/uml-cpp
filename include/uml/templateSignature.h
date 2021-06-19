#ifndef TEMPLATE_SIGNATURE_H
#define TEMPLATE_SIGNATURE_H

#include "element.h"
#include "templateParameter.h"
#include "sequence.h"

namespace UML {

    class TemplateableElement;

    class TemplateSignature : public Element {
        private:
            ID m_templateID;
            TemplateableElement* m_templatePtr;
            Sequence<TemplateParameter> m_ownedParameter;
        public:
            TemplateSignature();
            virtual ~TemplateSignature();
            TemplateableElement* getTemplate();
            void setTemplate(TemplateableElement* temp);
            Sequence<TemplateParameter>& getOwnedParameter();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_SIGNATURE;
            };
    };
}

#endif