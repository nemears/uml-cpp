#ifndef REDEFINABLE_ELEMENT_H
#define REDEFINABLE_ELEMENT_H

#include "classifier.h"

namespace UML {
    class RedefinableElement : public NamedElement {
        protected:
            Sequence<RedefinableElement>* m_redefinedElement;
            Sequence<Classifier>* m_redefinitionContext;
        public:
            RedefinableElement();
            ~RedefinableElement();
            RedefinableElement(const RedefinableElement& el);
            Sequence<RedefinableElement>& getRedefinedElements();
            Sequence<Classifier>& getRedefinitionContext();
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif