#ifndef TEMPLATEABLE_ELEMENT_H
#define TEMPLATEABLE_ELEMENT_H

#include "element.h"
#include "templateSignature.h"

namespace UML {

    class TemplateBinding;

    class TemplateableElement : virtual public Element {
        private:
            ID m_ownedTemplateSignatureID;
            TemplateSignature* m_ownedTemplateSignaturePtr;
            ID m_templateBindingID;
            TemplateBinding* m_templateBindingPtr;
        public:
            TemplateableElement();
            TemplateableElement(const TemplateableElement& el);
            virtual ~TemplateableElement();
            TemplateSignature* getOwnedTemplateSignature();
            void setOwnedTemplateSignature(TemplateSignature* signature);
            TemplateBinding* getTemplateBinding();
            void setTemplateBinding(TemplateBinding* binding);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATEABLE_ELEMENT;
            };
    };
}

#endif