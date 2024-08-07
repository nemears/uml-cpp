#pragma once

#include "namedElement.h"
#include "uml/set/set.h"

namespace UML {

    class Classifier;

    class RedefinableElement : virtual public NamedElement {
        protected:
            ReadOnlySet<RedefinableElement, RedefinableElement> m_redefinedElement = ReadOnlySet<RedefinableElement, RedefinableElement>(this);
            ReadOnlySet<Classifier, RedefinableElement> m_redefinitionContext = ReadOnlySet<Classifier, RedefinableElement>(this);
            void referenceErased(ID id) override;
            RedefinableElement();
        public:
            virtual ~RedefinableElement();
            ReadOnlySet<RedefinableElement, RedefinableElement>& getRedefinedElements();
            ReadOnlySet<Classifier, RedefinableElement>& getRedefinitionContext();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::REDEFINABLE_ELEMENT;
            };
    };
}
