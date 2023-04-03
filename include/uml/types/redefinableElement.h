#ifndef _UML_REDEFINABLE_ELEMENT_H_
#define _UML_REDEFINABLE_ELEMENT_H_

#include "namedElement.h"
#include "uml/set/set.h"

namespace UML {

    class Classifier;

    class RedefinableElement : virtual public NamedElement {
        protected:
            CustomReadOnlySet<RedefinableElement, RedefinableElement> m_redefinedElement = CustomReadOnlySet<RedefinableElement, RedefinableElement>(this);
            CustomReadOnlySet<Classifier, RedefinableElement> m_redefinitionContext = CustomReadOnlySet<Classifier, RedefinableElement>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            RedefinableElement();
        public:
            virtual ~RedefinableElement();
            ReadOnlySet<RedefinableElement, RedefinableElement>& getRedefinedElements();
            ReadOnlySet<Classifier, RedefinableElement>& getRedefinitionContext();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::REDEFINABLE_ELEMENT;
            };
    };
}

#endif