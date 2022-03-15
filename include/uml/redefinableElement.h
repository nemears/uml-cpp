#ifndef _UML_REDEFINABLE_ELEMENT_H_
#define _UML_REDEFINABLE_ELEMENT_H_

#include "namedElement.h"

namespace UML {

    class Classifier;

    class RedefinableElement : virtual public NamedElement {
        protected:
            Set<RedefinableElement, RedefinableElement> m_redefinedElement = Set<RedefinableElement, RedefinableElement>(this);
            Set<Classifier, RedefinableElement> m_redefinitionContext = Set<Classifier, RedefinableElement>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
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