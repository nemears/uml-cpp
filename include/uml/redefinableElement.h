#ifndef REDEFINABLE_ELEMENT_H
#define REDEFINABLE_ELEMENT_H

#include "sequence.h"

namespace UML {

    class Classifier;

    class RedefinableElement : virtual public NamedElement {
        friend class UmlManager;
        protected:
            Sequence<RedefinableElement> m_redefinedElement;
            Sequence<Classifier> m_redefinitionContext;
            void setManager(UmlManager* manager) override; // not polymorphic
        public:
            RedefinableElement();
            virtual ~RedefinableElement();
            RedefinableElement(const RedefinableElement& el);
            Sequence<RedefinableElement>& getRedefinedElements();
            Sequence<Classifier>& getRedefinitionContext();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::REDEFINABLE_ELEMENT;
            };
    };
}

#endif