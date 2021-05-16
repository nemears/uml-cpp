#ifndef REDEFINABLE_ELEMENT_H
#define REDEFINABLE_ELEMENT_H

#include "sequence.h"

namespace UML {

    class Classifier;

    class RedefinableElement : virtual public NamedElement {
        protected:
            Sequence<RedefinableElement> m_redefinedElement;
            Sequence<Classifier> m_redefinitionContext;
        public:
            RedefinableElement();
            virtual ~RedefinableElement();
            RedefinableElement(const RedefinableElement& el);
            Sequence<RedefinableElement>& getRedefinedElements();
            Sequence<Classifier>& getRedefinitionContext();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif