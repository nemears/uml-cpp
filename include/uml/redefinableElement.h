#ifndef REDEFINABLE_ELEMENT_H
#define REDEFINABLE_ELEMENT_H

#include "sequence.h"

namespace UML {

    class Classifier;

    class RedefinableElement : virtual public NamedElement {
        friend class UmlManager;
        protected:
            Sequence<RedefinableElement> m_redefinedElement = Sequence<RedefinableElement>(this);
            Sequence<Classifier> m_redefinitionContext = Sequence<Classifier>(this);
            void setManager(UmlManager* manager) override; // not polymorphic
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
        public:
            RedefinableElement();
            virtual ~RedefinableElement();
            RedefinableElement(const RedefinableElement& el);
            Sequence<RedefinableElement>& getRedefinedElements();
            Sequence<Classifier>& getRedefinitionContext();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::REDEFINABLE_ELEMENT;
            };
    };
}

#endif