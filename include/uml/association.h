#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include "classifier.h"
#include "relationship.h"

namespace UML {
    class Association : public Classifier, public Relationship {
        protected:
            Sequence<Property>* m_memberEnds;
            Sequence<Property>* m_ownedEnds;
            Sequence<Property>* m_navigableOwnedEnds;
        public:
            Association();
            ~Association();
            Sequence<Property>& getMemberEnds();
            Sequence<Property>& getOwnedEnds();
            Sequence<Property>& getNavigableOwnedEnds();
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType);
    };
}

#endif