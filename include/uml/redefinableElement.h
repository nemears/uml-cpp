#ifndef REDEFINABLE_ELEMENT_H
#define REDEFINABLE_ELEMENT_H

#include "namedElement.h"

namespace UML {

    class Classifier;

    class RedefinableElement : virtual public NamedElement {
        protected:
            Set<RedefinableElement, RedefinableElement> m_redefinedElement = Set<RedefinableElement, RedefinableElement>(this);
            Set<Classifier, RedefinableElement> m_redefinitionContext = Set<Classifier, RedefinableElement>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void init();
            void copy(const RedefinableElement& rhs);
            RedefinableElement();
        public:
            virtual ~RedefinableElement();
            RedefinableElement(const RedefinableElement& el);
            Set<RedefinableElement, RedefinableElement>& getRedefinedElements();
            Set<Classifier, RedefinableElement>& getRedefinitionContext();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::REDEFINABLE_ELEMENT;
            };
    };
}

#endif