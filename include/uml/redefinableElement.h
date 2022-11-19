#ifndef _UML_REDEFINABLE_ELEMENT_H_
#define _UML_REDEFINABLE_ELEMENT_H_

#include "namedElement.h"

namespace UML {

    class Classifier;

    class RedefinableElement : virtual public NamedElement {
        protected:
            CustomSet<RedefinableElement, RedefinableElement> m_redefinedElement = CustomSet<RedefinableElement, RedefinableElement>(this);
            CustomSet<Classifier, RedefinableElement> m_redefinitionContext = CustomSet<Classifier, RedefinableElement>(this);
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void init();
            RedefinableElement();
        public:
            virtual ~RedefinableElement();
            Set<RedefinableElement, RedefinableElement>& getRedefinedElements();
            Set<Classifier, RedefinableElement>& getRedefinitionContext();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::REDEFINABLE_ELEMENT;
            };
    };
}

#endif