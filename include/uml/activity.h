#ifndef ACTIVITY_H
#define ACTIVITY_H
#include "behavior.h"
#include "activityNode.h"
#include "activityEdge.h"

namespace UML {
    class Activity : public Behavior {
        protected:
            Sequence<ActivityNode> m_nodes;
            Sequence<ActivityEdge> m_edges;
            class AddNodeFunctor : public AbstractSequenceFunctor {
                public:
                    AddNodeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddEdgeFunctor : public AbstractSequenceFunctor {
                public:
                    AddEdgeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            Activity();
            ~Activity();
            Sequence<ActivityNode>& getNodes();
            Sequence<ActivityEdge>& getEdges();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::ACTIVITY;
            };
    };
}


#endif