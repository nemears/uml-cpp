#ifndef _UML_DIRECTED_RELATIONSHIP_H_
#define _UML_DIRECTED_RELATIONSHIP_H_

#include "relationship.h"

namespace UML {
    class DirectedRelationship : public Relationship {
        protected:
            Set<Element, DirectedRelationship> m_targets = Set<Element, DirectedRelationship>(this);
            Set<Element, DirectedRelationship> m_sources = Set<Element, DirectedRelationship>(this);
            void init();
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