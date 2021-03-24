#ifndef DIRECTEDRELATIONSHIP_H
#define DIRECTEDRELATIONSHIP_H

#include "relationship.h"

namespace UML {
    class DirectedRelationship : public Relationship {
        protected:
            Sequence<>* m_targets;
            Sequence<>* m_sources;
            // below is commented out because should be obsolete with function pointer add and remove functionality on sequence
            //void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
        public:
            DirectedRelationship();
            ~DirectedRelationship();
            Sequence<>& getTargets();
            Sequence<>& getSources();
            ElementType getElementType() override;
    };
}

#endif