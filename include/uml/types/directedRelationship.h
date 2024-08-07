#pragma once

#include "relationship.h"

namespace UML {
    class DirectedRelationship : public Relationship {
        protected:
            ReadOnlySet<Element, DirectedRelationship> m_targets = ReadOnlySet<Element, DirectedRelationship>(this);
            ReadOnlySet<Element, DirectedRelationship> m_sources = ReadOnlySet<Element, DirectedRelationship>(this);
            DirectedRelationship();
        public:
            virtual ~DirectedRelationship();
            ReadOnlySet<Element, DirectedRelationship>& getTargets();
            ReadOnlySet<Element, DirectedRelationship>& getSources();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DIRECTED_RELATIONSHIP;
            };
    };
}
