#ifndef PARAMETERABLE_ELEMENT_H
#define PARAMETERABLE_ELEMENT_H

#include "element.h"

namespace UML {

    class TemplateParameter;
    class TemplateParameterSubstitution;

    class ParameterableElement : virtual public Element {

        friend class TemplateParameter;
        friend class TemplateParameterSubstitution;

        private:
        public:
            ParameterableElement();
            ParameterableElement(const ParameterableElement& el);
            ~ParameterableElement();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PARAMETERABLE_ELEMENT;
            };
    };
}

#endif