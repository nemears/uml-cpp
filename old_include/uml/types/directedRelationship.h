#ifndef _UML_DIRECTED_RELATIONSHIP_H_
#define _UML_DIRECTED_RELATIONSHIP_H_

#include "relationship.h"

namespace UML {
    class DirectedRelationship : public Relationship {
        protected:
            CustomReadOnlySet<Element, DirectedRelationship> m_targets = CustomReadOnlySet<Element, DirectedRelationship>(this);
            CustomReadOnlySet<Element, DirectedRelationship> m_sources = CustomReadOnlySet<Element, DirectedRelationship>(this);
            DirectedRelationship();
        public:
            virtual ~DirectedRelationship();
            ReadOnlySet<Element, DirectedRelationship>& getTargets();
            ReadOnlySet<Element, DirectedRelationship>& getSources();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DIRECTED_RELATIONSHIP;
            };
    };
}

#endif