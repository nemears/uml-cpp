#ifndef _UML_DIRECTED_RELATIONSHIP_H_
#define _UML_DIRECTED_RELATIONSHIP_H_

#include "relationship.h"

namespace UML {
    class DirectedRelationship : public Relationship {
        protected:
            CustomSet<Element, DirectedRelationship> m_targets = CustomSet<Element, DirectedRelationship>(this);
            CustomSet<Element, DirectedRelationship> m_sources = CustomSet<Element, DirectedRelationship>(this);
            DirectedRelationship();
        public:
            virtual ~DirectedRelationship();
            Set<Element, DirectedRelationship>& getTargets();
            Set<Element, DirectedRelationship>& getSources();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DIRECTED_RELATIONSHIP;
            };
    };
}

#endif