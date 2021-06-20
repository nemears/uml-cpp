#ifndef TEMPLATE_BINDING_H
#define TEMPLATE_BINDING_H

#include "directedRelationship.h"

namespace UML {
    class TemplateBinding : public DirectedRelationship {
        private:
        public:
            TemplateBinding();
            TemplateBinding(const TemplateBinding& bind);
            ~TemplateBinding();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_BINDING;
            };
    };
}

#endif