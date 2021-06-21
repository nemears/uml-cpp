#ifndef TEMPLATE_BINDING_H
#define TEMPLATE_BINDING_H

#include "directedRelationship.h"
#include "templateableElement.h"

namespace UML {
    class TemplateBinding : public DirectedRelationship {
        private:
            ID m_boundElementID;
            TemplateableElement* m_boundElementPtr;
            ID m_signatureID;
            TemplateSignature* m_signaturePtr;
        public:
            TemplateBinding();
            TemplateBinding(const TemplateBinding& bind);
            ~TemplateBinding();
            TemplateableElement* getBoundElement();
            void setBoundElement(TemplateableElement* el);
            TemplateSignature* getSignature();
            void setSignature(TemplateSignature* signature);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_BINDING;
            };
    };
}

#endif