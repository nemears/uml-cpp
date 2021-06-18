#ifndef TEMPLATEABLE_ELEMENT_H
#define TEMPLATEABLE_ELEMENT_H

#include "element.h"
#include "templateSignature.h"

namespace UML {
    class TemplateableElement : virtual public Element {
        private:
            ID m_ownedTemplateSignatureID;
            TemplateSignature* m_ownedTemplateSignaturePtr;
        public:
            virtual ~TemplateableElement();
            TemplateSignature* getOwnedTemplateSignature();
            void setOwnedTemplateSignature(TemplateSignature* signature);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::TEMPLATEABLE_ELEMENT;
            };
    };
}

#endif