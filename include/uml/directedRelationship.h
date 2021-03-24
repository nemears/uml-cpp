#ifndef DIRECTEDRELATIONSHIP_H
#define DIRECTEDRELATIONSHIP_H

#include "relationship.h"

namespace UML {
    class DirectedRelationship : public Relationship {
        protected:
            Sequence<>* m_targets;
            Sequence<>* m_sources;
            // below is commented out because should be obsolete with function pointer add and remove functionality on sequence?
            //void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;

            // functor triggered on getTargets().add(el) and getSources().add(el)
            class AddRelatedElementFunctor : public AbstractSequenceFunctor {
                public:
                    AddRelatedElementFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            DirectedRelationship();
            ~DirectedRelationship();
            Sequence<>& getTargets();
            Sequence<>& getSources();
            ElementType getElementType() override;
    };
}

#endif