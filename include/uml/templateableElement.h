#ifndef TEMPLATEABLE_ELEMENT_H
#define TEMPLATEABLE_ELEMENT_H

#include "element.h"

namespace UML {
    class TemplateableElement : virtual public Element {
        private:

        public:
            virtual ~TemplateableElement();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif