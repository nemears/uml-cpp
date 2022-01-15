#ifndef DIRECTEDRELATIONSHIP_H
#define DIRECTEDRELATIONSHIP_H

#include "relationship.h"

namespace UML {
    class DirectedRelationship : public Relationship {

        friend class UmlManager;

        protected:
            Set<Element, DirectedRelationship> m_targets = Set<Element, DirectedRelationship>(this);
            Set<Element, DirectedRelationship> m_sources = Set<Element, DirectedRelationship>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void init();
            void copy(const DirectedRelationship& rhs);
            DirectedRelationship();
        public:
            DirectedRelationship(const DirectedRelationship& relationship);
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